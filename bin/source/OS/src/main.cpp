extern void Init();
extern void OS();
extern void fixNOW();

int main() {
    fixNOW();
    Init();
    OS();
    return 0;
}