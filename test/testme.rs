struct X {}
fn main() {
    let x = X{};
    f(x);
    g(x); 
}

fn f(x: X) {}
fn g(x: X) {}