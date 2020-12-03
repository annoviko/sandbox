#include <stdio.h>
#include <cstdlib>
#include <cstring>


struct node {
	node * m_left;
	node * m_right;
	int m_value;
};


struct tree {
private:
	node * m_root;

private:
	void insert(int p_val, node * p_node) {
		if (p_val > p_node->m_value) {
			if (p_node->m_right == NULL) {
				p_node->m_right = (node *) malloc(sizeof(node));
				memset(p_node->m_right, 0, sizeof(node));
				p_node->m_right->m_value = p_val;
			}
			else {
				insert(p_val, p_node->m_right);
			}
		}
		else {
			if (p_node->m_left == NULL) {
				p_node->m_left = (node *) malloc(sizeof(node));
				memset(p_node->m_left, 0, sizeof(node));
				p_node->m_left->m_value = p_val;
			}
			else {
				insert(p_val, p_node->m_left);
			}
		}
	}


	void remove(node * p_node) {
		if (p_node->m_left != NULL)
			remove(p_node->m_left);

		if (p_node->m_right != NULL)
			remove(p_node->m_right);

		free(p_node);
		p_node = NULL;
	}


	int sum(node * p_node) {
		int result = p_node->m_value;
		if (p_node->m_left != NULL)
			result += sum(p_node->m_left);

		if (p_node->m_right != NULL)
			result += sum(p_node->m_right);

		return result;
	}


	int height(node * p_node) {
		int right_height = 1;
		int left_height = 1;

		if (p_node->m_left != NULL) {
			left_height += height(p_node->m_left);
		}

		if (p_node->m_right != NULL) {
			right_height += height(p_node->m_right);
		}

		return left_height > right_height ? left_height : right_height;
	}

	int distance_to(int p_val, node * p_node) {
		if (p_val > p_node->m_value) {
			if (p_node->m_right != NULL) {
				int distance = distance_to(p_val, p_node->m_right);
				return (distance < 0) ? -1 : 1 + distance;
			}
		}
		else if (p_val == p_node->m_value) {
			return 0;
		}
		else if (p_val < p_node->m_value) {
			if (p_node->m_left != NULL) {
				int distance = distance_to(p_val, p_node->m_left);
				return (distance < 0) ? -1 : 1 + distance;
			}
		}

		return -1;
	}


public:
	bool create(int* p_arr, int len) {
		if (len == 0 || p_arr == NULL)
			return false;

		m_root = (node *) malloc(sizeof(node));
		memset(m_root, 0, sizeof(node));
		m_root->m_value = p_arr[0];

		for (int i = 1; i < len; i++)
			insert(p_arr[i], m_root);

		return true;
	}


	void destroy(void) {
		if (m_root == NULL)
			return;

		remove(m_root);
	}


	int sum(void) {
		return m_root == NULL ? 0 : sum(m_root);
	}


	int height(void) {
		return m_root == NULL ? 0 : height(m_root);
	}


	int distance(int p_val1, int p_val2) {
		if (!m_root)
			return -1;

		node * common_node = m_root;
		while(true) {
			if (p_val1 > common_node->m_value && p_val2 > common_node->m_value) {
				if (common_node != NULL)
					common_node = common_node->m_right;
				else
					return -1;
			}
			else if (p_val1 < common_node->m_value && p_val2 < common_node->m_value) {
				if (common_node != NULL)
					common_node = common_node->m_left;
				else
					return -1;
			}
			else if (p_val1 == common_node->m_value && p_val2 == common_node->m_value) {
				return 0;
			}
			else {
				int distance1 = distance_to(p_val1, common_node);
				int distance2 = distance_to(p_val2, common_node);

				return (distance1 < 0 || distance2 < 0) ? -1 : distance1 + distance2;
			}
		}
	}

};



int main(int argc, char * argv[]) {
	int values[] = {5, 2, 1, 3, 6, 9, 7};
	int length = 7;

	tree tr;
	tr.create(values, length);
	printf("sum: %d\n", tr.sum());
	printf("height: %d\n", tr.height());
	printf("[2] - [6]: %d\n", tr.distance(2, 6));
	printf("[1] - [3]: %d\n", tr.distance(1, 3));
	printf("[1] - [6]: %d\n", tr.distance(1, 6));
	printf("[1] - [7]: %d\n", tr.distance(1, 7));
	printf("[6] - [9]: %d\n", tr.distance(6, 9));
	printf("[3] - [9]: %d\n", tr.distance(3, 9));
	printf("[5] - [5]: %d\n", tr.distance(5, 5));
	printf("[1] - [1]: %d\n", tr.distance(1, 1));
	printf("[10] - [2]: %d\n", tr.distance(10, 2));
	tr.destroy();

	return 0;
}
