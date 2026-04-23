struct __maia_runtime_value {
  int tag;
};

static void* __maia_runtime_alloc_value() {
  __maia_runtime_value* value = new __maia_runtime_value();
  return (void*)value;
}

extern void* __maia_lambda1_capture1(int c1);

void* __maia_lambda1_capture1(int c1) {
  (void)c1;
  return __maia_runtime_alloc_value();
}

int main() {
  return 0;
}