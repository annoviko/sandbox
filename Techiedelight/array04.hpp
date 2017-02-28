#pragma once


#include <unordered_set>
#include <type_traits>


template <typename Container>
decltype(auto) findDuplicate(const Container & pCollection) {
	using const_reference_container_type = decltype(*std::begin(pCollection));
	using const_container_type = typename std::remove_reference<const_reference_container_type>::type;
	using container_type = typename std::remove_const<const_container_type>::type;
	
	container_type duplicate;
	
	std::unordered_set<container_type> container;
	
	for (auto & element : pCollection) {
		if (container.find(element) != std::end(container)) {
			duplicate = element;
			break;
		}
		else {
			container.insert(element);
		}
	}
	
	return duplicate;
}


void printCollection(const auto & pCollection) {
	std::cout << "[ ";
	std::for_each(std::begin(pCollection), std::end(pCollection), [](auto & pElemenet) { std::cout << pElemenet << " "; });
	std::cout << "]" << std::endl;
}


void displayDuplicate1(const auto & pCollection) {
	printCollection(pCollection);
	auto duplicate = findDuplicate(pCollection);
	std::cout << "Duplicate: " << duplicate << std::endl << std::endl;
}
