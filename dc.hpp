using namespace std;

template<typename Tin, typename Tout>
Tout dc(Tin input, bool (*basecase)(Tin), Tout (*solve)(Tin),
        vector<Tin> (*divide)(Tin), Tout (*conquer)(vector<Tout>)) {

        if (basecase(input)) {
                return(solve(input));
        } else {
                auto subproblems = divide(input);

                std::transform(subproblems.begin(),
                               subproblems.end(),
                               subproblems.begin(),
                               [&](Tin x) {
                        auto res = dc(x, basecase, solve, divide, conquer);
                        return(res);
                });

                auto result = conquer(subproblems);
                return(result);
        }
};
