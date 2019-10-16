package com.telerik.metadata.nodes.cache;

import com.telerik.metadata.TreeNode;

public interface NodesCache {

    public void addNode(String className, TreeNode node);

    public TreeNode getNode(String className);

}
