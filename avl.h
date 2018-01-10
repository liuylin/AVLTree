#include<iostream>
using namespace std;

template<class K,class V>
struct AVLTreeNode
{
	K _key;
	V _value;
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;
	int _bf;//平衡因子

	AVLTreeNode(const K& key,const V& value)
		:_key(key)
		, _value(value)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _bf(0)
	{}
};

template<class K,class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	AVLTree()
		:_root(NULL)
	{}

	void InOrder()
	{
		_InOrder(_root);
	}

	bool Insert(const K& key, const V& value)
	{
		if (_root == NULL)
		{
			_root = new Node(key, value);
			return true;
		}
		Node * cur = _root;
		Node * parent = NULL;
		while (cur)
		{
			if (cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return false;
			}
		}
		cur = new Node(key, value);
		if (parent->_key < key)
		{
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
		}
		//更新平衡因子
		//如果parent的平衡因子变为0，则说明这棵树的高度没变，不用更新它的父节点
		//如果平衡因子变为1或-1，要继续更新父节点
		//如果变为2或-2，则需要旋转
		while (parent)
		{
			if (cur == parent->_left)
			{
				parent->_bf--;
			}
			else
			{
				parent->_bf++;
			}
			if (parent->_bf == 0)
			{
				break;
			}
			else if (parent->_bf == 1 || parent->_bf == -1)  //当前平衡因子等于1或-1时，需要向上查看其父节点的平衡因子
			{
				cur = parent;
				parent = cur->_parent;
			}
			else//平衡因子等于2或者-2，此时需要进行调节
			{
				if (parent->_bf == 2)
				{
					if (cur->_bf == 1)
					{
						RotateL(parent);
					}
					else
					{
						RotateRL(parent);
					}
				}
				else
				{
					if (cur->_bf == -1)
					{
						RotateR(parent);
					}
					else
					{
						RotateLR(parent);
					}
				}
				break;
			}
		}
		return true;
	}


	bool IsBalance()
	{
		int height = 0;
		return _IsBalance(_root,height);
	}
protected:
	//O(N)判断一棵树是不是平衡二叉树
	bool _IsBalance(Node* root, int& height)
	{
		if (root == NULL)
		{
			height = 0;
			return true;
		}
		int leftHeight = 0;
		if (_IsBalance(root->_left, leftHeight) == false)
			return false;
		int rightHeight = 0;
		if (_IsBalance(root->_right, rightHeight) == false)
			return false;
		height = leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;

		if (rightHeight - leftHeight != root->_bf)
		{
			cout << "平衡因子:"<<root->_bf<<"  平衡因子异常:" << root->_key << endl;
			return false;
		}
		return abs(leftHeight - rightHeight) < 2;
	}


	//右旋
	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;

		subL->_right = parent;
		Node* ppNode = parent->_parent;
		parent->_parent = subL;
		
		if (ppNode == NULL)
		{
			_root = subL;
			subL->_parent = NULL;
		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = subL;
			}
			else
			{
				ppNode->_right = subL;
			}
			subL->_parent = ppNode;
		}
		subL->_bf = parent->_bf = 0;
	}

	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;

		subR->_left = parent;
		Node* ppNode = parent->_parent;
		parent->_parent = subR;

		if (ppNode == NULL)
		{
			_root = subR;
			subR->_parent = NULL;
		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = subR;
			}
			else
			{
				ppNode->_right = subR;
			}
			subR->_parent = ppNode;
		}
		subR->_bf = parent->_bf = 0;
	}

	//右左双旋
	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;
		RotateR(parent->_right);
		RotateL(parent);
		if (bf == 1)
		{
			subR->_bf = 0;
			parent->_bf = -1;
		}
		else if (bf == -1)
		{
			subR->_bf = 1;
			parent->_bf = 0;
		}
		else
		{
			subR->_bf = parent->_bf = 0;
		}
		subRL->_bf = 0;
	}

	//左右双旋
	void RotateLR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;
		RotateL(parent->_left);
		RotateR(parent);
		if (bf == 1)
		{
			parent->_bf = 0;
			subL->_bf = -1;
		}
		else if (bf == -1)
		{
			subL->_bf = 0;
			parent->_bf = 1;
		}
		else
		{
			subL->_bf = parent->_bf = 0;
			subLR->_bf = 0;
		}
	}

	//O(N^2)
	bool _IsBalance(Node* root)
	{
		if (root == NULL)
			return true;

		int leftHeight = Height(root->_left);
		int rightHeight = Height(root->_right);

		return abs(leftHeight - rightHeight) < 2
			&& _IsBalance(root->_left)
			&& _IsBalance(root->_right);
	}

	int Height(Node* root)
	{
		if (root == NULL)
		return 0;
		int left = Height(root->_left);
		int right = Height(root->_right);

		return left > right ? left + 1 : right + 1;
	}

protected:
	//中序遍历
	void _InOrder(Node* root)
	{
		if (root == NULL)
			return;
		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}

private:
	Node* _root;
};

void TestAVL()
{
	/*int a[] = { 16, 30, 7, 11, 9, 26, 18, 14, 19 };*/
	//特殊场景
	int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	AVLTree<int, int>tree;
	for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
	{
		tree.Insert(a[i], i);
		cout << "IsBalance?:" << tree.IsBalance() << "->InSert:"<<a[i]<<endl;
	}
	tree.InOrder();
	cout << endl;
	cout << "IsBalance?:" << tree.IsBalance() << endl;
	cout << endl;
}