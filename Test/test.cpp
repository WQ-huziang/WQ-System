#include <iostream>
#include <fstream>
using namespace std;

void test(ostream &fout) {
	fout << "test\n";
}

int main() {
	FILE *fp;
	fp = stdout;
	fprintf(fp, "test\n");
	if (fp != stdout)
		fclose(fp);
	fprintf(fp, "test\n");
}