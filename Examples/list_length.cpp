#include <iostream>
#include <string>


struct node {
public:
	static int INDEXER;
public:
	int 	index;
	node * 	next;
	
public:
	node(void) { index = ++INDEXER; }
};


int node::INDEXER = 0;


node * generate(const int length, const int loop) {
	node * start = new node();
	
	node * cur = start;
	cur->next = nullptr;
	
	node * loop_node = nullptr;
	for (int i = 1; i < length; i++) {		
		if (loop == i) {
			loop_node = cur;
		}
		
		node * next_node = new node();
		cur->next = next_node;
		
		cur = next_node;
		cur->next = nullptr;
	}
	
	cur->next = loop_node;

	return start;
}


void print(node * start, int length) {
	node * cur = start;
	for (int i = 0; i < length; i++, cur = cur->next) {
		std::cout << "[" << cur->index << "] -> ";
	}
	
	if (cur->next != nullptr) {
		std::cout << "[" << cur->index << "]";
	}
	else {
		std::cout << "[nullptr]";
	}
	
	std::cout << std::endl;
}


int length(node * start) {
	node * cur1 = start;
	node * cur2 = start;

	bool loop = true;
	while(true) {
		cur1 = cur1->next;
		cur2 = cur2->next->next;
		
		if (cur1 == cur2) {
			break;
		}
	}
#if 0
	while(true) {
		if ( (cur2->next == nullptr) || (cur2->next->next == nullptr) ) { 
			break; 
		}
		
		cur1 = cur1->next;
		
		if ( (cur2 != start) && (cur2->next == cur1) ) {
			loop = true;
			break;
		}
		
		cur2 = cur2->next->next;
		
		std::cout << "Step: cur1 '" << cur1->index << "', cur2 '" << cur2->index << "'" << std::endl;
		
		if (cur1 == cur2) { 
			loop = true;
			break;
		}
	}
#endif

	if (!loop) {
		int total_length = 0;
		for (cur1 = start; cur1->next != nullptr; cur1 = cur1->next) { total_length++; }
		return total_length;
	}
	else {
		std::cout << "List consists loop (meet place '" << cur1->index << "')." << std::endl;
	
	
		int loop_length = 1;
		for (cur2 = cur1; cur2->next != cur1; cur2 = cur2->next) { loop_length++; }
		std::cout << "Loop length: " << loop_length << "." << std::endl;
		
#if 0
		int length_without_loop = 0;
		for (cur2 = start; cur2 != cur1; cur2 = cur2->next) { length_without_loop++; }
		
		int length_with_loop = length_without_loop + 1;
		for (cur2 = cur1; cur2->next != cur1; cur2 = cur2->next) { length_with_loop++; }
		std::cout << "Length with loop: " << length_with_loop << std::endl;
		
		return loop_length + (length_with_loop - loop_length);
#endif
		
		int unloop_length = 0;
		for(cur2 = start; cur1 != cur2; cur1 = cur1->next, cur2 = cur2->next) {	
			unloop_length++;
		}
		
		return unloop_length + loop_length;
	}
}


int main(int argc, char * argv[]) {
	if (argc != 3) {
		std::cout << "Usage example: <util> <length> <loop>" << std::endl;
	}
	
	node * start = generate(std::stoi(argv[1]), std::stoi(argv[2]));
	print(start, std::stoi(argv[1]));
	
	std::cout << "List length: " << length(start) << std::endl;
	
	return 0;
}

