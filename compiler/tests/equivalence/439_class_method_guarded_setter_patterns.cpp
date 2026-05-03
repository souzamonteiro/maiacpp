class GuardedSetterPatterns {
public:
    GuardedSetterPatterns() : life(50), strength(75) {}

    void setLife(int v) {
        if (v >= 0 && v <= 100) {
            life = v;
        }
    }

    void setStrength(int v) {
        if (v >= 1 && v <= 99) {
            strength = v;
        }
    }

    int getLife() {
        return life;
    }

    int getStrength() {
        return strength;
    }

private:
    int life;
    int strength;
};

int main() {
    GuardedSetterPatterns value;

    value.setLife(80);
    value.setStrength(99);
    if (value.getLife() != 80) return 1;
    if (value.getStrength() != 99) return 1;

    value.setLife(-1);
    value.setStrength(100);
    if (value.getLife() != 80) return 1;
    if (value.getStrength() != 99) return 1;

    return 0;
}
