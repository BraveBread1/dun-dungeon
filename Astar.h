//#ifndef Astar_h_
//#define Astar_h_
//
//#include"CommonFunc.h"
//#include"Entity.h"
//
//struct Node
//{
//    Node();
//    int i;
//    int j;
//    int g;
//    int f;
//    int h;
//    int closed;
//    Node* parent;
//    Node* next;
//    void free();
//};
//
//class Astar
//{
//private:
//    Node* nodetail;
//    Node nodehead;
//    Entity* owner;
//
//public:
//    Astar();
//    void resetAStar();
//    void addNodeToOpenList(Node* node);
//    void buildRouteMap(int sx, int sy, int ex, int ey, bool** hasSolid, int pj, int pi, Entity* head);
//    Node* getSmallestNode();
//
//    int isBlocked(int j, int i, bool** hasSolid, int pj, int pi, Entity* head);
//    Node* findNextMove(int j, int i);
//    void createAStarRoute(Entity* e, int j, int i, int* dj, int* di, bool** hasSolid, int pj, int pi, Entity* head);
//    
//};
//
//
//
//#endif