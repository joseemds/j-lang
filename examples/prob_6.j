type BinTreeNode = {
  key : Int;
  left, right : BinTreeNode;    
};

func create_bin_tree_root(key : Int) : BinTreeNode {
  -- val root : BinTreeNode = BinTreeNode {key: key, left: null, right: null};
  return BinTreeNode {key: key, left: null, right: null};
}

func insert_bin_tree(root : BinTreeNode, key : Int) : BinTreeNode {
  if (root == null) {
    return create_bin_tree_root(key);
  }
  if (key <= root.key) {
    root.left = insert_bin_tree(root.left, key);
  } else {
    root.right = insert_bin_tree(root.right, key);
  }
  return root;
}

func get_lower_bound(root : BinTreeNode) : Int {
  if (root == null) {
    print("Void Tree.");
    return 0;
  }
  val level, lowest : Int = 0, root.key;
  while (root.left != null) {
    root = root.left;
    level = level + 1;
    lowest = root.key;
  }

  print("Nível: " + level);
  return lowest;
}

func get_upper_bound(root : BinTreeNode) : Int {
  if (root == null) {
    print("Void Tree.");
    return 0;
  }
  val level, greatest : Int = 0, root.key;
  while (root.right != null) {
    root = root.right;
    level = level + 1;
    greatest = root.key;
  }

  print("Nível: " + level);
  return greatest;
}

 func create_bin_tree_from_list(list : [Int]) : BinTreeNode {
  val root : BinTreeNode;
  if (length(list) != 0) { root = create_bin_tree_root(pop(list)); }
  while (length(list) != 0) {
    root = insert_bin_tree(root, pop(list));
  }

  return root;
}