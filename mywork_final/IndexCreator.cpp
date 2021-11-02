#include "IndexCreator.h"
    

IndexCreator::IndexCreator(){

}

IndexCreator::~IndexCreator(){

}

//创建一个节点对象

BPTNode* IndexCreator::new_BPTNode(){
    BPTNode* node = (BPTNode*)malloc(sizeof(BPTNode));
    if (node == NULL)
        return NULL;
    for (int i = 0; i < 2 * M - 1; i++)
        node->datanodes[i].value = -1;
    for (int i = 0; i < 2 * M; i++)
        node->childs[i] = NULL;
    node->num = 0;
    node->is_leaf = true;
    node->left = NULL;
    node->right = NULL;
    return node;

}
//创建B+树

BPTNode* IndexCreator::create_BPTree(){
    BPTNode* node = new_BPTNode();
    if (NULL == node)
        return NULL;
    node->left = node;
    node->right = node;
    return node;    
}

//为col列属性创建数据节点
KeyNode IndexCreator::CreateDataNode(Record record,int col){
    KeyNode dataNode;
    dataNode.primary_key = record.getkey();
    dataNode.value = record.getColValue(col);
    return dataNode;    
}

//插入未满的节点中
void IndexCreator::BPTree_Insert_NotFull(BPTNode* node, const KeyNode& DataNode){
    if (node->is_leaf) {
        int pos = node->num;
        while (pos >= 1 && DataNode.value < node->datanodes[pos - 1].value) {
            node->datanodes[pos] = node->datanodes[pos - 1];
            pos--;
        }
        node->datanodes[pos] = DataNode;
        node->num++;
    } else {
        int pos = node->num;
        while (pos > 0 && DataNode.value < node->datanodes[pos - 1].value) {
            pos--;
        }
        if (2 * M - 1 == node->childs[pos]->num) {
            BPTree_Split_Child(node, pos, node->childs[pos]);
            if (DataNode.value > node->datanodes[pos].value)
                pos++;
        }
        BPTree_Insert_NotFull(node->childs[pos], DataNode);
    }

}

//节点分裂
int IndexCreator::BPTree_Split_Child(BPTNode* parent, int pos, BPTNode* child){
    BPTNode* new_child = new_BPTNode();
    if (!new_child)
        return -1;
    new_child->is_leaf = child->is_leaf;
    new_child->num = M - 1;
    for (int i = 0; i < M - 1; i++)
        new_child->datanodes[i] = child->datanodes[i + M];
    if (!new_child->is_leaf) {
        for (int i = 0; i < M; i++)
            new_child->childs[i] = child->childs[i + M];
    }
    child->num = M - 1;
    if (child->is_leaf)
        child->num++;//如果是叶节点，保留中间的关键码
    for (int i = parent->num; i > pos; i--)
        parent->childs[i + 1] = parent->childs[i];
    parent->childs[pos + 1] = new_child;
    for (int i = parent->num - 1; i >= pos; i--)
        parent->datanodes[i + 1] = parent->datanodes[i];
    parent->datanodes[pos] = child->datanodes[M - 1];
    parent->num++;

    //叶节点情况，需要更新指针
    if (child->is_leaf) {
        new_child->right = child->right;
        child->right->left = new_child;
        child->right = new_child;
        new_child->left = child;
    }
    return 1;
}

//插入新节点
BPTNode* IndexCreator::BPTree_Insert(BPTNode* root, const Record& record, int col){
    KeyNode DataNode = CreateDataNode(record, col);
    if (!root) {
        return NULL;
    }
    if (root->num == 2 * M - 1) {
        BPTNode* node = new_BPTNode();
        if (!node) {
            return NULL;
        }
        node->is_leaf = false;
        node->childs[0] = root;
        BPTree_Split_Child(node, 0, root);
        BPTree_Insert_NotFull(node, DataNode);
        return node;
    } else {
        BPTree_Insert_NotFull(root, DataNode);
        return root;
    }
}

//搜索符合条件的记录
vector<int64_t> IndexCreator::SearchValueRange(BPTNode* root, int64_t left, int64_t right, int col){

    //cout << root->num<<endl;
    cout << "正在寻找符合条件的记录！"<< endl;
    vector<int64_t> KeyVec;//用于存储符合条件的记录key值
    int num = 0;
    //若left==right,输出col属性值=left的记录主键
    if (left==right){
        int64_t value = left;
        cout << "定值搜索("<< value <<")"<<endl;
        if (!root){
            cout << "B+树索引结构为空！";
            return KeyVec;
        }
        BPTNode *node = root;
        cout << "正在向下搜索,寻找叶子节点"<<endl;
        while (!node->is_leaf) { //若不是叶节点，则往下搜索
            int pos = 0;
            while (pos < node->num && value > node->datanodes[pos].value)
                pos++;
            node = node->childs[pos];
        }
        cout << "到达叶子节点"<<endl;
        //到达叶节点，顺着right指针往右搜索
        while (node) {
            for (int i = 0; i < node->num; i++) {
                if (node->datanodes[i].value > value)
                    return KeyVec;
                if (node->datanodes[i].value == value)
                    KeyVec.push_back(node->datanodes[i].primary_key);
            }
            node = node->right;
    }
    }
    //给定区间，进行区间搜索
    else if (left<right){
        cout << "区间搜索("<< left << "," << right<<")"<<endl;

        BPTNode *node_left = root;
        BPTNode *node_right = root;
        cout << "正在向下搜索,寻找叶子节点"<<endl;
        //往下搜索，找到left对应的叶子节点
        while (!node_left->is_leaf) {
            //cout << "左节点正在向下搜索叶子节点"<<endl;
            int pos = 0;
            while (pos < node_left->num && left > node_left->datanodes[pos].value)
                pos++;
            node_left = node_left->childs[pos];
        }
        //往下搜索，找到right对应的叶子节点
        while (!node_right->is_leaf) {
            //cout << "右节点正在向下搜索叶子节点"<<endl;
            int pos = node_right->num;
            while (pos > 0 && right < node_right->datanodes[pos - 1].value)
                pos--;
            node_right = node_right->childs[pos];
        }
        cout << "到达叶子节点"<<endl;
        cout << "左叶子节点的第一个值为:"<< node_left->datanodes[0].value<<endl;
        cout << "右叶子节点的第一个值为:"<< node_right->datanodes[0].value<<endl;
        //移动node_left指针直到node_right
        while (node_left != node_right) {
            if(node_left==NULL){
                //cout << "叶子节点为空!" <<endl;
                return KeyVec;
            }
            for (int i = 0; i < node_left->num; i++) {
                if (node_left->datanodes[i].value >= left)
                    KeyVec.push_back(node_left->datanodes[i].primary_key);
            }
            node_left = node_left->right;
        }
        //cout << "node_left和node_right相遇"<<endl;
        //当node_left和node_right相遇
        for (int i = 0; i < node_left->num; i++) {
            if (left <= node_left->datanodes[i].value && node_left->datanodes[i].value <= right)
                KeyVec.push_back(node_left->datanodes[i].primary_key);
        }       
    }
    else{
        cout << "请输入正确的区间（left, right）" << endl;
        throw "error";
    }

    return KeyVec;

}

//保存索引文件
CLStatus IndexCreator::SaveIndexFile(BPTNode *root,int col){
    char index_file_path[20];
    sprintf(index_file_path, "%s%d", INDEX_FILE, col);
    //判断是否已经存在，存在则删除原有文件
    if (Is_IndexFile_Existed(index_file_path)) {
        if (remove(index_file_path ) == -1) {
            cout << "删除原有索引文件失败";
            return CLStatus(-1, 0);
        }
    }
    int fd = open(index_file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
        return CLStatus(-1, 0);
    WriteBPTNode(fd, root);
    close(fd);
    return CLStatus(0,0);
}

//将B+树节点写入文件
CLStatus IndexCreator::WriteBPTNode(int &fd, BPTNode *node) {
    if (node == NULL || fd == -1) {
        return CLStatus(-1, 0);
    }
    if (write(fd, node, sizeof(BPTNode)) == -1)
        throw "In WriteBPTNode(),write error";
    for (int i = 0; i <= node->num; i++)
        WriteBPTNode(fd, node->childs[i]);
    return CLStatus(0, 0);
}

//读取索引文件
BPTNode* IndexCreator::ReadIndexFile(int col){
    char index_file_path[20];
    sprintf(index_file_path, "%s%d", INDEX_FILE, col);
    BPTNode* leaf_node_pre = NULL; //保存前一个叶节点
    if (Is_IndexFile_Existed(index_file_path)){
        int fd = open(index_file_path, O_RDONLY, S_IRUSR | S_IWUSR);
        if (fd == -1)
            throw "error when open indexfile!";
        BPTNode *node = ReadBPTNode(fd, leaf_node_pre);
        close(fd);
        //cout <<"in ReadIndexFile::"<<"num in root:" <<node->num<<endl;
        return node;
    }
    else
        throw "there is no indexfile for col";
}

//读取B+树节点
BPTNode *IndexCreator::ReadBPTNode(int &fd, BPTNode* &leaf_node_pre) {

    BPTNode *node = new_BPTNode();
    if (node == NULL)
        return NULL;
    if (read(fd, node, sizeof(BPTNode)) == -1)
        return NULL;
    if (node->is_leaf) {
        if (leaf_node_pre == NULL)
            leaf_node_pre = node;
        else
        {
            leaf_node_pre->right = node;
            leaf_node_pre=node;
        }
        return node;
    }
    for (int i = 0; i <= node->num; i++)
        node->childs[i] = ReadBPTNode(fd,leaf_node_pre);
    return node;
}

//判断col是否存在索引文件
bool IndexCreator::Is_IndexFile_Existed(char pathname[]){
    //判断路径是否存在
    if (access(pathname, F_OK) == 0)
        return true;
    return false;
}