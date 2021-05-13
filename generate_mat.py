import random


def gen_mat(filename, n, m):
    f = open(filename, "w")

    f.write(f"row={n} col={m}\n")
    for i in range(n):
        for j in range(m):
            f.write(f"{random.randint(0, 100)}\t")
        f.write("\n")
    f.close()


l = random.randint(10, 20)
n = random.randint(10, 20)
m = random.randint(10, 20)
gen_mat("a.txt", l, n)
gen_mat("b.txt", n, m)