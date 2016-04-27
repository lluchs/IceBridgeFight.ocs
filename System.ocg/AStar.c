/* Pathfinder */

// Does A* on the map. `start` and `goal` are {x, y} proplists.
global func AStarMap(proplist start, proplist goal, int step)
{
	if (start.x % step != goal.x % step || start.y % step != goal.y % step)
		FatalError("AStarMap: Goal not reachable with this step size.");
	var ops = {
		distance = this._AStarMapDistance,
		cost = this._AStarMapDistance,
		successors = this._AStarMapSuccessors,
		step = step,
	};
	return AStar(start, goal, ops);
}

global func _AStarMapDistance(proplist a, proplist b)
{
	// Manhattan distance
	return Abs(a.x - b.x) + Abs(a.y - b.y);
}

global func _AStarMapPathFree(proplist a, proplist b)
{
	return PathFree(a.x, a.y, b.x, b.y);
}

global func _AStarMapSuccessors(proplist a)
{
	var successors = [], pt;
	if (_AStarMapPathFree(a, (pt = {x = a.x + this.step, y = a.y}))) PushBack(successors, pt);
	if (_AStarMapPathFree(a, (pt = {x = a.x, y = a.y + this.step}))) PushBack(successors, pt);
	if (_AStarMapPathFree(a, (pt = {x = a.x - this.step, y = a.y}))) PushBack(successors, pt);
	if (_AStarMapPathFree(a, (pt = {x = a.x, y = a.y - this.step}))) PushBack(successors, pt);
	return successors;
}

// Generic A* implementation.
global func AStar(start, goal, proplist ops)
{
	var state = {
		open = [[ops->distance(start, goal), 0, start]],
		closed = [],
		goal = goal,
	};
	var current;
	while (GetLength(state.open))
	{
		current = HeapExtract(state.open);
		if (DeepEqual(current[2], goal))
		{
			// Reconstruct the path.
			var path = [goal];
			while (current = current[3])
				PushFront(path, current[2]);
			return path;
		}
		PushBack(state.closed, current[2]);
		_AStarExpand(state, ops, current);
	}
	return nil;
}

global func _AStarExpand(proplist state, proplist ops, array current)
{
	/* Log("open: %v, closed: %v", state.open, state.closed); */
	/* Log("current: %v", current); */
	for (var successor in ops->successors(current[2]))
	{
		/* Log(" - successor: %v", successor); */
		// Skip successor if it's in the closed list.
		var i = 0, el;
		while ((el = state.closed[i++]) && !DeepEqual(el, successor));
		if (el)
			continue;
		var cost = current[1] + ops->cost(current[2], successor);
		// Find successor in the open list.
		i = 0;
		while ((el = state.open[i++]) && !DeepEqual(el[2], successor));
		if (el && el[1] <= cost)
			continue;
		successor = [cost + ops->distance(successor, state.goal), cost, successor, current];
		if (el)
		{
			el[:] = successor;
			HeapDecreaseKey(state.open, i-1);
		}
		else
			HeapInsert(state.open, successor);
	}
}

/* Binary Min-Heap */

// A heap is an array of [key, value] array-tuples.
global func Heapify(array heap, int i)
{
	var min = i, size = GetLength(heap);
	var left = 2*i+1, right = 2*i+2;
	if (left < size && heap[left][0] < heap[min][0])
		min = left;
	if (right < size && heap[right][0] < heap[min][0])
		min = right;
	if (min != i)
	{
		ArraySwap(heap, min, i);
		Heapify(heap, min);
	}
}

global func BuildHeap(array a)
{
	for (var i = GetLength(a)/2-1; i >= 0; --i)
		Heapify(a, i);
}

global func HeapDecreaseKey(array heap, int i)
{
	var parent;
	while (i > 0 && heap[i][0] < heap[(parent = (i-1)/2)][0])
	{
		ArraySwap(heap, i, parent);
		i = parent;
	}
}

global func HeapInsert(array heap, array kv)
{
	var i = GetLength(heap);
	heap[i] = kv;
	HeapDecreaseKey(heap, i);
}

global func HeapExtract(array heap)
{
	var item = heap[0], last = PopBack(heap);
	if (GetLength(heap))
	{
		heap[0] = last;
		Heapify(heap, 0);
	}
	return item;
}

global func ArraySwap(array a, int i, int j)
{
	var tmp = a[i];
	a[i] = a[j];
	a[j] = tmp;
}
