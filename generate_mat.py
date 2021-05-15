import random


def gen_mat(filename, n, m):
    f = open(filename, "w")

    f.write(f"row={n} col={m}\n")
    for i in range(n):
        for j in range(m):
            f.write(f"{random.randint(0, 100)}\t")
        f.write("\n")
    f.close()


l = random.randint(50, 100)
n = random.randint(50, 100)
m = random.randint(50, 100)
gen_mat("a.txt", l, n)
gen_mat("b.txt", n, m)