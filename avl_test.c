#include "avl_tree.h"



int main(int argc, char *argv[]) {

    clock_t start, end;
    double time_taken;

    struct Del_Data {
        struct avlnode node;
        int Key;
        int Value;
    };

    struct Del_Data query = (struct Del_Data) {};

    int get_Data(struct avlnode* node) {
        struct Del_Data *dat = (struct Del_Data*) node;
        return dat->Key;
    }

    int _comp_function (const void *a, const void *b) {

        return ((struct Del_Data*) a)->Key - ((struct Del_Data*) b)->Key;
    }

    struct avlnode *test = NULL;

    srand(time(NULL));

    //int keys[] = {375,408,178,454,292,82,466,445,304,258,107,9,319,269,205,143,181};
    int _size = 10000000;//sizeof(keys) / sizeof(int);
/*
    for(int i = 0; i < _size; i++) {
        int k = rand() % 100;
        //printf("%d ", k);
        keys[i] = k;
        //dels[i] = rand() % _size;
    }
    //printf("\n");
*/
    start = clock();
    for(int i = 0; i < _size; i++) {

        struct Del_Data *data = (struct Del_Data*) malloc(sizeof(struct Del_Data));
        //data->node = (struct avlnode) {};

        int h_rnd = rand() << 16;
        h_rnd ^= rand();

        //printf("Key is [%d] \n", h_rnd);//keys[i]);

        data->Key = h_rnd;//keys[i];
        data->Value = 0;
        avl_Insert(&test, data, _comp_function);
        //avl_PrintTreeVisual(&test, 0);//, get_Data);
        //printf("-----------\n");
    }

    end = clock();
    time_taken = ((double)(end - start) / CLOCKS_PER_SEC);
    //avl_PrintTreeVisual(&test, 0);//, get_Data);
    printf("Insert Time taken: %.3f seconds\n", time_taken);

/*
    for(int i =0; i < _size; i++) { /// This is for Root Deletions
        printf("Key is [%d]\n", keys[i]);
        avl_Delete(&test, keys[i], get_Data);
        avl_PrintTreeVisual(&test, 0, get_Data);
    }
*/

/// Cleaning up
    avl_reset_stack();
    start = clock();
    while(avl_Del_Leaf());
    free(test);
/*
    while(test) {
        query.Key = _get_key(&test);
        avl_Del_Node(&query, _comp_function);
    }
*/
    end = clock();

    time_taken = ((double)(end - start) / CLOCKS_PER_SEC);

    printf("Delete Time taken: %.3f seconds\n", time_taken);


    return 0;
}
