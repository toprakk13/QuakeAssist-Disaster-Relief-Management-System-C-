#include "RequestQueue.h"
#include <new> // for std::nothrow

RequestQueue::RequestQueue()
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    // start with a small default capacity
    resize(4);
}

RequestQueue::RequestQueue(int initialCapacity)
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    if (initialCapacity < 1) {
        initialCapacity = 4;
    }
    resize(initialCapacity);
}

RequestQueue::~RequestQueue() {
    delete[] data;
}

bool RequestQueue::isEmpty() const {
    return count == 0;
}

bool RequestQueue::isFull() const {
    return count == capacity;
}

int RequestQueue::size() const {
    return count;
}

int RequestQueue::nextIndex(int idx) const {
    if (capacity == 0) return 0;
    return (idx + 1) % capacity;
}

bool RequestQueue::enqueue(const Request& req) {
    //Implement enqueue function as explained in the PDF.
    (void)req; // suppress unused warning until implemented
    if (isFull()) {resize(capacity*2);}
    
    data[rear]=req;
    rear = nextIndex(rear);
    count++;
    return true;
    
    
}

bool RequestQueue::dequeue(Request& outReq) {
    //Implement dequeue function as explained in the PDF.
    (void)outReq; // suppress unused warning until implemented
    if(isEmpty()){return false;}
    else{
        outReq=data[front];
        front=(front+1)%capacity;
        count--;
        if(count==0){clear();}
        return true;
    }
    
}

bool RequestQueue::peek(Request& outReq) const {
    //Implement peek function as explained in the PDF.
    if (isEmpty()) {return false;}   
    outReq = data[front];
    return true;
}

void RequestQueue::clear() {
    front = 0;
    rear = 0;
    count = 0;
}

bool RequestQueue::removeById(const std::string& id) {
    //Implement removeById function as explained in the PDF.
    (void)id; // suppress unused warning until implemented
    int removeIndex=-1;
    if(isEmpty()){return false;}
    for(int i=0;i<count;i++){
    int idx=(front+i)%capacity;
    if(data[idx].getId()==id){
        removeIndex=idx;
        break;
    }
    
    }
    if(removeIndex==-1){return false;}

    int current=removeIndex;
    while(current!=rear){
        int next = nextIndex(current);
        if (next == rear) {break;} 
        data[current] = data[next];
        current = next;
    }
    rear=(rear-1+capacity)%capacity;
    count--;
    if(count==0){clear();}
    return true;
    
    
    
    
}

bool RequestQueue::resize(int newCapacity) {
    //Implement resize function as explained in the PDF.
    if (newCapacity < count) {return false;} 
    Request* newData = new Request[newCapacity];

    for (int i = 0; i < count; i++) {
        int idx = (front + i) % capacity;
        newData[i] = data[idx];
    }

    delete[] data;
    data = newData;
    capacity = newCapacity;
    front = 0;
    rear = count;

    return true;
}
