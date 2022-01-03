#ifndef MAP_H_
#define MAP_H_

typedef struct {
    void* key;
    void* data;
} Map;

Map* MapCreate(void* keyArr, void* dataArr);
void MapDestroy(Map* map);

#endif /* MAP_H_ */