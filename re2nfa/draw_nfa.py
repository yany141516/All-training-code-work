from graphviz import Digraph

def load_edges(filename):
    edges = []
    start = None
    end = None

    with open(filename, "r") as f:
        for line in f:
            line = line.strip()
            parts = line.split()

            if parts[0] == "start":
                start = parts[1]
                continue
            if parts[0] == "end":
                end = parts[1]
                continue

            u, symbol, v = parts
            edges.append((u, v, symbol))

    return start, end, edges


def draw_nfa(start, end, edges, outfile="nfa"):
    dot = Digraph("NFA", format="png")
    dot.attr(rankdir="LR")

    # Start node
    dot.node("start", shape="plaintext")
    dot.edge("start", start)

    # Normal nodes + double circle final node
    dot.node(end, shape="doublecircle")

    # Draw edges
    for u, v, symbol in edges:
        if symbol == "#":
            label = "ε"
        else:
            label = symbol
        dot.edge(u, v, label=label)

    dot.render(outfile, cleanup=True)
    print(f"Generated {outfile}.png")


if __name__ == "__main__":
    start, end, edges = load_edges("re2nfa\edges.txt")
    draw_nfa(start, end, edges)
