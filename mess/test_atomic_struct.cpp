#include <atomic>

template<class T>
struct MyStruct{
  T x; T y;
	struct MyChildStruct {
		T z;
		T a;
		T b;
	};
};

struct X{
  std::atomic<MyStruct<int>::MyChildStruct> myStruct;
};

int main(){
  X x;
	MyStruct<int>::MyChildStruct s = atomic_load(&x.myStruct);
}
