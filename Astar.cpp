//#include"Astar.h"
//
//Node::Node()
//{
//    j = 0;
//    i = 0;
//    g = 0;
//    h = 0;
//    f = 0;
//    closed = 0;
//    next = NULL;
//    parent = NULL;
//}
//
//void Node::free()
//{
//	j = 0;
//	i = 0;
//	g = 0;
//	h = 0;
//	f = 0;
//	closed = 0;
//	next = NULL;
//	parent = NULL;
//}
//
//Astar::Astar()
//{
//	nodehead.j = 0;
//	nodehead.i = 0;
//	nodehead.g = 0;
//	nodehead.h = 0;
//	nodehead.f = 0;
//	nodehead.closed = 0;
//	nodehead.next = NULL;
//	nodehead.parent = NULL;
//	nodetail = &nodehead;
//}
//
//void Astar::resetAStar()
//{
//	Node* n;
//	while (nodehead.next != NULL)
//	{
//		n = nodehead.next;
//		nodehead.next = n->next;
//		n->free();
//        delete n;
//		n = NULL;
//	}
//	nodetail = &nodehead;
//}
//
//void Astar::addNodeToOpenList(Node* node)
//{
//	Node* n;
//	if (node->f != -1)
//	{
//		for (n = nodehead.next; n != NULL; n = n->next)
//		{
//			if (n->j == node->j && n->i == node->i)
//			{
//				if (node->f < n->f)
//				{
//					n->f = node->f;
//					n->g = node->g;
//					n->h = node->h;
//					n->closed = 0;
//					n->parent = node->parent;
//				}
//
//				node->free();
//                delete node;
//				node = NULL;
//
//				return;
//			}
//		}
//
//		nodetail->next = node;
//		nodetail = node;
//	}
//	else
//	{
//		node->free();
//        delete node;
//		node = NULL;
//	}
//}
//
//Node* Astar::getSmallestNode()
//{
//    Node* smallest;
//    Node *n;
//
//    smallest = NULL;
//    n = NULL;
//
//    for (n = nodehead.next; n != NULL; n = n->next)
//    {
//        if (!n->closed && n->f != -1 && (smallest == NULL || n->f < smallest->f))
//        {
//            smallest = n;
//        }
//    }
//
//    return smallest;
//}
//
//void Astar::buildRouteMap(int sj, int si, int ej, int ei, bool** hasSolid, int pj, int pi, Entity* head)
//{
//    int j, i, count;
//    Node* start;
//    Node* currentNode;
//    Node* newNode;
//
//    start = new Node;
//    start->j = sj;  
//    start->i = si;
//    addNodeToOpenList(start);
//
//    currentNode = start;
//
//    count = 0;
//
//    while (1)
//    {
//        for (i = -1; i <= 1; i++)
//        {
//            for (j = -1; j <= 1; j++)
//            {
//                if (j == 0 && i == 0)
//                {
//                    continue;
//                }
//
//                if (currentNode->j + j < 0 || currentNode->i + i < 0 || currentNode->j + j >= LEVEL_COLS || currentNode->i + i >= LEVEL_ROWS)
//                {
//                    continue;
//                }
//
//                newNode = new Node;
//                    
//                newNode->j = currentNode->j + j;
//                newNode->i = currentNode->i + i;
//
//                if (!isBlocked  (newNode->j, newNode->i, hasSolid, pj, pi, head))
//                {
//                    if (j != 0 && i != 0)
//                    {
//                        newNode->g = currentNode->g + 14;
//                    }
//                    else
//                    {
//                        newNode->g = currentNode->g + 10;
//                    }
//
//                    newNode->h = 10 * (abs(newNode->j - ej) + abs(newNode->i - ei));
//
//                    newNode->f = newNode->g + newNode->h;
//                }
//                else
//                {
//                    newNode->f = -1;
//                    newNode->g = -1;
//                    newNode->h = -1;
//                }
//
//                newNode->parent = currentNode;
//
//                addNodeToOpenList(newNode);
//
//                if (++count >= LEVEL_ROWS * LEVEL_COLS)
//                {
//                    return;
//                }
//            }
//        }
//
//        currentNode = getSmallestNode();
//
//        if (!currentNode)
//        {
//            break;
//        }
//
//        currentNode->closed = 1;
//
//        if (currentNode->j == ej && currentNode->i == ei)
//        {
//            break;
//        }
//    }
//}
//
//int Astar::isBlocked(int j, int i, bool** hasSolid, int pj, int pi, Entity* head)
//{
//    Entity* e;
//
//    if (i == owner->getPosI() && j == owner->getPosJ()) return 0;
//
//    if (hasSolid[i][j] == 1 || (j == pj && i == pi))
//    {
//        return 1;
//    }
//
//    for (Entity* e = head; e != NULL; e = e->next)
//    {
//        if (e->getPosI() == i && e->getPosJ() == j)
//        {
//            return 1;
//        }
//    }
//
//    return 0;
//}
//
//Node* Astar::findNextMove(int j, int i)
//{
//    Node* n;
//
//    for (n = nodehead.next; n != NULL; n = n->next)
//    {
//        if (n->j == j && n->i == i)
//        {
//            return n->parent;
//        }
//    }
//
//    return NULL;
//}
//
//void Astar::createAStarRoute(Entity* e, int j, int i, int* dj, int* di, bool** hasSolid, int pj, int pi, Entity* head)
//{
//    Node* n;
//
//    owner = e;
//
//    *dj = 0;
//    *di = 0;
//
//    resetAStar();
//
//    buildRouteMap(j, i, e->getPosJ(), e->getPosI(), hasSolid, pj, pi, head);
//
//    n = findNextMove(e->getPosJ(), e->getPosJ());
//
//    if (n != NULL)
//    {
//        *dj = n->j - e->getPosJ();
//        *di = n->i - e->getPosJ();
//    }
//}
//
