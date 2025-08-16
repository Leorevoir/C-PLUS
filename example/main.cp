def dummy_function() -> int
{
    x = 5;

    if x > 0 {
        return x;
    } else {
        return 0;
    }
}

def main() -> int
{
    if dummy_function() {
        return 0;
    }
    return 84;
}
