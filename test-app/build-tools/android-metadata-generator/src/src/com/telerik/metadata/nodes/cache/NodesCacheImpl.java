package com.telerik.metadata.nodes.cache;

import com.telerik.metadata.TreeNode;

import java.util.HashMap;
import java.util.Map;

public class NodesCacheImpl implements NodesCache {
    private static final NodesCacheImpl ourInstance = new NodesCacheImpl();

    private final Map<String, TreeNode> nodesCache;

    public static NodesCache getInstance() {
        return ourInstance;
    }

    private NodesCacheImpl() {
        this.nodesCache = new HashMap<>();
    }

    public void addNode(String className, TreeNode node) {
        nodesCache.put(className, node);
    }

    public TreeNode getNode(String className) {
        return nodesCache.get(className);
    }
}
