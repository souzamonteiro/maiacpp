const char* __new__Error(const char* message) {
  return message;
}

int main() {
  try {
    throw __new__Error("expected error");
  } catch (const char* error) {
    return error[0] == 'e' ? 0 : 1;
  }
  return 1;
}
