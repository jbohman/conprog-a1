#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <cstdio>

using namespace Gecode;

class Queens : public Script {
protected:
    IntVarArray q;
public:
    Queens(const SizeOptions& opt) : q(*this, opt.size() * opt.size(), 0, 1) {
        Matrix<IntVarArray> matrix(q, opt.size(), opt.size());

        // Start values
        for (int i = 0; i < opt.size(); ++i) {
            for (int j = 0; j < opt.size();++j) {
                dom(*this, matrix(i, j), 0, 1);
            }
            // rel(*this, matrix(i, 0), IRT_EQ, 1);
        }

        // Distinct col
        for (int i = 0; i < opt.size(); ++i) {
            count(*this, matrix.col(i), 1, IRT_EQ, 1);
            count(*this, matrix.row(i), 1, IRT_EQ, 1);
        }
       
        // Diagonal
        for (int j = 0; j < opt.size(); ++j) {
            IntVarArray dia(*this, opt.size()-j);
            for (int i = 0; i+j < opt.size(); ++i) {
                dia[i] = matrix(i+j, i);
            }
            count(*this, dia, 1, IRT_LQ, 1);
        }

        for (int j = 0; j < opt.size(); ++j) {
            IntVarArray dia(*this, opt.size()-j);
            for (int i = 0; i+j < opt.size(); ++i) {
                dia[i] = matrix(i, i+j);
            }
            count(*this, dia, 1, IRT_LQ, 1);
        }

        for (int j = 0; j < opt.size(); ++j) {
            IntVarArray dia(*this, opt.size()-j);
            for (int i = 0; i+j < opt.size(); ++i) {
                dia[i] = matrix((opt.size() - 1) - (i + j), i);
            }
            count(*this, dia, 1, IRT_LQ, 1);
        }

        for (int j = 0; j < opt.size(); ++j) {
            IntVarArray dia(*this, opt.size()-j);
            for (int i = 0; i+j < opt.size(); ++i) {
                dia[i] = matrix((opt.size() - 1) - (i + 0), i + j);
            }
            count(*this, dia, 1, IRT_LQ, 1);
        }

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
            os << q[i] << " ";
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
    opt.size(10);
    //opt.propagation(Queens::PROP_DISTINCT);
    //opt.propagation(Queens::PROP_BINARY, "binary", "only binary disequality constraints");
    //opt.propagation(Queens::PROP_MIXED, "mixed", "single distinct and binary disequality constraints");
    //opt.propagation(Queens::PROP_DISTINCT, "distinct", "three distinct constraints");
    opt.parse(argc,argv);
    Script::run<Queens, DFS, SizeOptions>(opt);
    return 0;
}
