type BinTree = {
    key : Int;
    left, right : BinTree;    
};

func create_bin_tree_from_list(list : [Int]) : BinTree {
    val root : BinTree;
    if (length(list) != 0) { root.key = pop(list); }
    while (length(list) != 0) {
        insert_bin_tree(root, pop(list));
    }

    return root;
}