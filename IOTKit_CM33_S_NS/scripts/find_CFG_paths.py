def dfs(current_node, remaining_steps, target_node, graph, current_path, all_paths):
    # Add the current node to the path
    current_path.append(current_node)
    
    # If we have exactly 0 steps left and are at the target node, record the path
    if remaining_steps == 0 and current_node == target_node:
        all_paths.append(list(current_path))  # Store a copy of the valid path
    elif remaining_steps > 0:
        # Explore all neighbors of the current node
        for neighbor in graph[current_node]:
            dfs(neighbor, remaining_steps - 1, target_node, graph, current_path, all_paths)
    
    # Backtrack: remove the current node from the path before returning
    current_path.pop()

# Main function to find the number of paths and print them
def find_paths_from_A_to_10(graph):
    start_node = 'A'
    target_node = '10'
    steps = 20
    all_paths = []  # To store all valid paths
    dfs(start_node, steps, target_node, graph, [], all_paths)
    
    # Print all paths and their count
    print(f"Number of paths from {start_node} to {target_node} in exactly {steps} transitions: {len(all_paths)}")
    # for path in all_paths:
    #     print(" -> ".join(path))

# Example graph in adjacency list format
graph = {
    'A': ['B', 'C', 'D'],
    'B': ['A', 'E', 'F'],
    'C': ['G', 'H'],
    'D': ['A', 'I', 'J'],
    'E': ['G', 'K'],
    'F': ['B', 'L'],
    'G': ['I', 'M'],
    'H': ['J', 'N'],
    'I': ['D', 'O'],
    'J': ['D', 'P'],
    'K': ['E', '10'],
    'L': ['F', '10'],
    'M': ['G', '10'],
    'N': ['H', '10'],
    'O': ['I', '10'],
    'P': ['J', '10'],
    '10': []
}


# Example usage
find_paths_from_A_to_10(graph)
