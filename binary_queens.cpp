#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>

using namespace Gecode;

class Queens : public Script {
protected:
    IntVarArray q;
public:
    Queens(const SizeOptions& opt) : q(*this, opt.size() * opt.size(), 0, 1) {
        Matrix<IntVarArray> matrix(q, opt.size(), opt.size());

        // Start values
        for (int i = 0; i < opt.size(); ++i) {
            rel(*this, matrix(i, 0), IRT_EQ, 1);
        }

        // Distinct col
        for (int i = 0; i < opt.size(); ++i) {
            distinct(*this, matrix.col(i), ICL_DEF);
        }
       
        // Diagonal


        branch(*this, q, INT_VAR_SIZE_MIN, INT_VAL_MIN);
    }

    Queens(bool share, Queens& s) : Script(share, s) {
        q.update(*this, share, s.q);
    }

    virtual Space* copy(bool share) {
        return new Queens(share, *this);
    }

    virtual void print(std::ostream& os) const {
        os << "queens\t";
        for (int i = 0; i < q.size(); i++) {
            os << q[i] << ", ";
            if ((i+1) % 10 == 0) {
                os << std::endl << "\t";
            }
        }
        os << std::endl;
    }
};

int main(int argc, char* argv[]) {
    SizeOptions opt("Queens");
    //opt.iterations(500);
    opt.size(100);
    //opt.propagation(Queens::PROP_DISTINCT);
    //opt.propagation(Queens::PROP_BINARY, "binary", "only binary disequality constraints");
    //opt.propagation(Queens::PROP_MIXED, "mixed", "single distinct and binary disequality constraints");
    //opt.propagation(Queens::PROP_DISTINCT, "distinct", "three distinct constraints");
    opt.parse(argc,argv);
    Script::run<Queens, DFS, SizeOptions>(opt);
    return 0;
}
