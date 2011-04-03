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

            branch(*this, q, INT_VAR_RND, INT_VAL_MAX);
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
                os << q[i];
                if ((i+1) % (int)sqrt(q.size()) == 0) {
                    os << std::endl << "\t";
                }
            }
            os << std::endl;
        }
};

int main(int argc, char* argv[]) {
    SizeOptions opt("Queens");
    opt.size(10);
    opt.parse(argc,argv);
    Script::run<Queens, DFS, SizeOptions>(opt);
    return 0;
}
