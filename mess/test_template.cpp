template<class T>
void whatever(T a, T b) {}

int main() {
  whatever<float>(1, 2.0); // correct

  float a = 1.0;
  whatever(a, 2.0); // wrong
  whatever(2.0, a); // wrong

  whatever(1.0, 2); // wrong
  return 0;
}
