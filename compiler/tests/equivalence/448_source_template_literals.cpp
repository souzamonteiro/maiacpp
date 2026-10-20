int main()
{
    const char* scalar = "this.failureMemo = new Set();";
    const char* array = "items = new Item[count];";
    const char* escaped = "\" = new Set(); \\";
    if (scalar[19] != 'n' || scalar[28] != ';' || scalar[29] != 0) return 1;
    if (array[8] != 'n' || array[23] != ';' || array[24] != 0) return 2;
    if (escaped[4] != 'n' || escaped[15] != '\\' || escaped[16] != 0) return 3;
    return 0;
}
