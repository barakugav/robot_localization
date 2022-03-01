#include "localizator.h"
#include "utils.hpp"

void Localizator::init(const Polygon &scene) {
	infoln("[Localizator] init...");
	sorted_by_max.clear();
	rtree.clear();

	/* Calculate all trapezoids */
	trapezoider.calc_trapezoids(scene);

	/* Fill trapezoids data structure and calculate min and max opening */
	for (auto it = trapezoider.trapezoids_begin(); it != trapezoider.trapezoids_end(); ++it)
		it->second.calc_min_max_openings();

	debugln("[Localizator] Trapezoids openings:");
	for (auto it = trapezoider.trapezoids_begin(); it != trapezoider.trapezoids_end(); ++it) {
		auto const &t = it->second;
		debugln("\tT" << t.get_id() << " [" << t.opening_min << ", " << t.opening_max << "]");
	}

	/* Populate the array of trapezoids sorted by their max opening. used for fast queries with one measurement */
	for (auto it = trapezoider.trapezoids_begin(); it != trapezoider.trapezoids_end(); ++it)
		sorted_by_max.push_back(it->first);
	sort(sorted_by_max.begin(), sorted_by_max.end(), [this](const auto &t1, const auto &t2) {
		return trapezoider.get_trapezoid(t1)->second.opening_max < trapezoider.get_trapezoid(t2)->second.opening_max;
	});
	debugln("[Localizator] sorted_by_max:");
	for (const auto &t_id : sorted_by_max) {
		const auto &trapezoid = trapezoider.get_trapezoid(t_id)->second;
		debugln("\tT" << trapezoid.get_id() << " [" << trapezoid.opening_min << ", " << trapezoid.opening_max << "]");
	}

	/* Populate interval tree of trapezoids, where each interval is [min opening, max opening] used for fast queries
	 * with two measurements. */
	for (auto it = trapezoider.trapezoids_begin(); it != trapezoider.trapezoids_end(); ++it) {
		const auto &trapezoid = it->second;
		TrapezoidRTreePoint min(CGAL::to_double(trapezoid.opening_min));
		TrapezoidRTreePoint max(CGAL::to_double(trapezoid.opening_max));
		rtree.insert(TrapezoidRTreeValue(TrapezoidRTreeSegment(min, max), it->first));
	}
}

void Localizator::query(const Kernel::FT &d, std::vector<Polygon> &res) const {
	/* Single measurement query. Perform binary search on the sorted array for output sensitive running time */
	infoln("[Localizator] Single measurement query (d = " << d << "):");
	auto it =
		std::lower_bound(sorted_by_max.begin(), sorted_by_max.end(), d, [this](const auto &trapezoid, const auto &d) {
			return trapezoider.get_trapezoid(trapezoid)->second.opening_max < d;
		});

	std::vector<Polygon> res_local;
	for (; it != sorted_by_max.end(); ++it) {
		const auto &trapezoid = trapezoider.get_trapezoid(*it)->second;
		debugln("\tT" << trapezoid.get_id() << " [" << trapezoid.opening_min << ", " << trapezoid.opening_max << "]");
		trapezoid.calc_result_m1(d, res_local);
	}
	infoln("[Localizator] result consist of " << res_local.size() << " polygons.");

	for (auto &p : res_local)
		res.push_back(std::move(p));
}

void Localizator::query(const Kernel::FT &d1, const Kernel::FT &d2, Arrangement &res) const {
	/* Double measurement query. Use the interval tree for output sensitive running time */
	infoln("[Localizator] Double measurement query (d1 = " << d1 << ", d2 = " << d2 << "):");
	const Kernel::FT d = d1 + d2;
	TrapezoidRTreePoint a(d), b(d);
	TrapezoidRTreeSegment query_interval(a, b);
	std::vector<TrapezoidRTreeValue> res_vals;
	rtree.query(boost::geometry::index::intersects(query_interval), std::back_inserter(res_vals));

	for (const TrapezoidRTreeValue &rtree_val : res_vals) {
		const auto &trapezoid = trapezoider.get_trapezoid(rtree_val.second)->second;
		debugln("\tT" << trapezoid.get_id() << " [" << trapezoid.opening_min << ", " << trapezoid.opening_max << "]");
		// TODO
	}
}
