extern void Init();
extern void OS();
extern void FixNOW();

int main() {
    FixNOW();
    Init();
    OS();
    return 0;
}