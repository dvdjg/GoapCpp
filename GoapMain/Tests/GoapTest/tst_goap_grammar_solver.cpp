#include "gtest/gtest.h"
#include <list>
#include <unordered_map>
#include <future>
#include <thread>
#include "termcolor/termcolor.hpp"
#include "explicit_ptr.h"
#include "goaplibrary.h"
#include "basicmath.h"
#include "log_hook.h"

#include "goap/istate.h"
#include "goap/iscopetimer.h"
#include "goap/isequencer.h"

#include "goap/istateiterator.h"

#include "grammar_solver.h"

#include "newptr.h"

using namespace goap;
using namespace std;
using namespace termcolor;

class GoapGrammarSolverTest : public ::testing::Test
{
public:
    launch launch = launch::async; // launch::async launch::deferred
    typedef list<IPlanningAction::CPtr> lst_plan_type;
    GoapGrammarSolverTest() {
    }

protected:
    static void SetUpTestCase() {
    }
    static void TearDownTestCase() {
    }
    virtual void SetUp() {
        LOG_CONF::singleton().setLevel(DEBUG);
    }
    virtual void TearDown() {
    }
};


TEST_F(GoapGrammarSolverTest, TestGrammarSolver)
{
    grammar_solver grammarSolver;
    ISequencer::Ptr seq1 = NewPtr<ISequencer>();
    const string strExpression("3210");
    seq1->pushString(strExpression, 1.0f);
    auto it = seq1->iterator();
    IState::New initialState{ {"4", 1}};
    initialState->putStateIterator("strExpression", it);
    grammarSolver.grammar_plan(initialState, { {"eval", 1} });
    lst_plan_type plan = grammarSolver.makePlan();


    auto solver1 = grammarSolver;
    LOG(INFO) << "Plan: " << planToOStream(solver1.plan(), solver1.initialState());
}

