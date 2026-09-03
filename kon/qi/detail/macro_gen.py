#!/usr/bin/env python3
def gen_call_each(limit: int):
    print("// clang-format off")
    for n in range(1, limit):
        output0 = f"#define KON_CALL_EACH{n}(F,{','.join(f'_{i}' for i in range(n))})"
        output1 = f"{', '.join(f'F(_{i})' for i in range(n))}"
        print(f"{output0} {output1}")


def gen_call_va(limit: int):
    print("// clang-format off")
    for n in range(1, limit):
        output0 = f"KON_CALL_VA({n},{','.join(f'_{i}' for i in range(n))})"
        print(output0)


if __name__ == "__main__":
    gen_call_each(128)
    # gen_call_va(128)
    # pass
