
struct bruh {
int x;
char c;
char* s;
int *y;
char* strings[2][3][4];
};

struct bruh2 {
    int y;
    struct bruh b;
    struct bruh *bp;
};

struct bruh3 {

    int z;
    struct bruh2 b;

};

struct bruhhh {

    int a;
    struct bruh b[4];
    struct bruh2 b2;
    struct bruh3 b3;

};

void main() {
struct bruhhh bhhh; struct bruhhh* bp; struct bruhhh bhhh2; struct bruhhh* bp2; int x;

bp = &bhhh;

bp2 = &bhhh2;

(*bp).b3.b.b.x = 1;

(*bp).b3.b.y = 2;

(*bp2).b3.b.b = (*bp).b3.b.b;

(*bp2).b2.b = (*bp).b3.b.b;

(*bp2).b[2].x = 5;

(*bp).b2.bp = &((*bp).b[2]);

(*bp).b[3].c = 'c';

(*bp).b[2].s = (char*)"hello";

(*bp).b[1].y = &((*bp2).b3.b.b.x);

(*bp).b[2].strings[0][2][2] = (char*)"Sam";

//bhhh.b[2].strings[0][2][2] = (char*)"Sam";

print_i((*bp).b3.b.b.x);
print_c('\n');
print_i((*bp).b3.b.y);
print_c('\n');
print_i((*bp2).b[2].x);
print_c('\n');
print_c((*bp).b[3].c);
print_c('\n');
print_s((*bp).b[2].s);
print_c('\n');
print_s((*bp).b[2].strings[0][2][2]);




}