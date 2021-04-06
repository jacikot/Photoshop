#include "Formater.h"
#include <algorithm>

void Formater::swap() {
	int end = height / 2 * width;

	for (auto i = pixels.begin(), k = i + pixels.size() - width; i != pixels.begin() + end; i += width) {
		auto j = i + width;
		std::swap_ranges(i, j, k);
		k -= width;
	}
}

