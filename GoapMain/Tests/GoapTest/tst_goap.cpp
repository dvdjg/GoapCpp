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

#include "backing_a_pie.h"
#include "hanoi_tower_solver.h"
#include "state_change_solver.h"

#include "newptr.h"

using namespace goap;
using namespace std;
using namespace termcolor;

class GoapTest : public ::testing::Test
{
public:
    static const float REF_TEMP;
    launch launch = launch::async; // launch::async launch::deferred
    typedef list<IPlanningAction::CPtr> lst_plan_type;
    GoapTest() {
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

const float GoapTest::REF_TEMP = 300;


TEST_F(GoapTest, TestNumericComparer)
{
    LOG(INFO) <<  magenta << "TestNumericComparer. 01" << reset << endl;
    IState::Ptr initialState = NewPtr<IState>()->assign(
            { {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP},  {"OwenIsOn", false}, {"Credits", 10} });

    IState::Ptr targetState = NewPtr<IState>()->assign(
            {{"PieIsReadyForEat", true}});

    IState::Ptr state1 = NewPtr<IState>()->assign(
            { {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP},  {"OwenIsOn", false}, {"Credits", 10}, {"EggIsOnBowl", false} });

    IState::Ptr state2 = NewPtr<IState>()->assign(
            { {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP},  {"OwenIsOn", false}, {"Credits", 10}, {"EggIsOnBowl", true} });

    IState::Ptr state3 = NewPtr<IState>()->assign(
            { {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP},  {"OwenIsOn", false}, {"Credits", 10}, {"EggIsOnBowl", true}, {"PieIsReadyForEat", true} });

    IState::Ptr backingHelper = NewPtr<IState>()->assign(
            {{"EggIsOnBowl", true}, {"ButterIsOnBowl", true}, {"FlourIsOnBowl", true}, {"IngredientsAreMixed", true}, {"PieIsBaked", true}});

    IPlanningStateComparer::Ptr numericalComparer = NewPtr<IPlanningStateComparer>(NUMERICSTATECOMPARER_SINGLETON);

    float distance  = numericalComparer->distance(initialState, backingHelper);
    float distance1 = numericalComparer->distance(state1, backingHelper);
    float distance2 = numericalComparer->distance(state2, backingHelper);
    bool bdistance2 = numericalComparer->enough(state2, targetState);

    EXPECT_GT(distance, distance2);
    EXPECT_GT(distance1, distance2);
    EXPECT_EQ(false, bdistance2);

    float distance3 = numericalComparer->distance(state3, targetState);
    EXPECT_EQ(0, distance3);

    bool bdistance3 = numericalComparer->enough(state3, targetState);
    EXPECT_EQ(true, bdistance3);
}

TEST_F(GoapTest, TestStateSolverOwenIsOn)
{
    state_change_solver solver;
    solver.setInputSequence({"OwenIsOn"});
    solver.setOutputSequence({"OwenIsOn"});
    lst_plan_type plan = solver.makePlan( { {"OwenIsOn", false} }, { {"OwenIsOn", true} } );
    EXPECT_NE(0, plan.size());
    LOG(INFO) << "State Plan OwenIsOn: " << planToOStream(solver.plan(), solver.initialState());
}

TEST_F(GoapTest, TestStateSolverOwenTemperature)
{
    state_change_solver solver;
    solver.setInputSequence({"OwenTemperature"});
    solver.setOutputSequence({"OwenTemperature"});
    lst_plan_type plan = solver.makePlan( { {"OwenTemperature", 300} }, { {"OwenIsOn", true} } );
    EXPECT_NE(0, plan.size());
    LOG(INFO) << "State Plan OwenIsOn: " << planToOStream(solver.plan(), solver.initialState());
}

TEST_F(GoapTest, TestTowerSolver)
{
    future<hanoi_tower_solver> f1 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lst_plan_type plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3} }, { {"C1", 1}, {"C2", 2}, {"C3", 3} }, 3);
        EXPECT_EQ(7, plan.size());
        return tower_solver;
    });
    future<hanoi_tower_solver> f2 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lst_plan_type plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4} }, 4);
        EXPECT_EQ(15, plan.size());
        return tower_solver;
    });
    future<hanoi_tower_solver> f3 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lst_plan_type plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5} }, 5);
        EXPECT_EQ(31, plan.size());
        return tower_solver;
    });
    future<hanoi_tower_solver> f4 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lst_plan_type plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5}, {"A6", 6} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6} }, 6);
        EXPECT_EQ(63, plan.size());
        return tower_solver;
    });

    LOG(INFO) << magenta << "Waiting..." << reset << flush;
    f1.wait();
    f2.wait();
    f3.wait();
    f4.wait();

    auto solver1 = f1.get();
    LOG(INFO) << "Plan: " << planToOStream(solver1.plan(), solver1.initialState());
    auto solver2 = f2.get();
    LOG(INFO) << "Plan: " << planToOStream(solver2.plan(), solver2.initialState());
    auto solver3 = f3.get();
    LOG(INFO) << "Plan: " << planToOStream(solver3.plan(), solver3.initialState());
    auto solver4 = f4.get();
    LOG(INFO) << "Plan: " << planToOStream(solver4.plan(), solver4.initialState());

    LOG(INFO) << magenta << "Computed." << reset << flush;
}

TEST_F(GoapTest, TestTowerSolver255)
{
    future<hanoi_tower_solver> f5 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lst_plan_type plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5}, {"A6", 6}, {"A7", 7} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6}, {"C7", 7} }, 7);
        EXPECT_EQ(127, plan.size());
        return tower_solver;
    });
    future<hanoi_tower_solver> f6 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lst_plan_type plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5}, {"A6", 6}, {"A7", 7}, {"A8", 8} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6}, {"C7", 7}, {"C8", 8} }, 8);
        EXPECT_EQ(255, plan.size());
        return tower_solver;
    });
    future<hanoi_tower_solver> f7 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lst_plan_type plan = tower_solver.makePlan( { {"A3", 1}, {"A4", 2}, {"A5", 3}, {"A6", 4}, {"A7", 5}, {"B6", 6}, {"B7", 7} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6}, {"C7", 7} }, 7);
        EXPECT_EQ(160, plan.size());
        return tower_solver;
    });
    f5.wait();
    f6.wait();
    f7.wait();
    auto solver5 = f5.get();
    LOG(INFO) << "Plan: " << planToOStream(solver5.plan(), solver5.initialState());
    auto solver6 = f6.get();
    LOG(INFO) << "Plan: " << planToOStream(solver6.plan(), solver6.initialState());
    auto solver7 = f7.get();
    LOG(INFO) << "Plan: " << planToOStream(solver7.plan(), solver7.initialState());
}

enum class ValueEvolution {
    NONE,
    APPEARED,
    DISSAPEARED,
    DISCOVERED,
    DISCOVERED_INC,
    DISCOVERED_DEC,
    INC,
    DEC,
    ACCEL_INC,
    ACCEL_DEC,
    MODIFIED
};

const char* ValueEvolution2Name[] = {
    "NONE",
    "APPEARED",
    "DISSAPEARED",
    "DISCOVERED",
    "DISCOVERED_INC",
    "DISCOVERED_DEC",
    "INC",
    "DEC",
    "ACCEL_INC",
    "ACCEL_DEC",
    "MODIFIED",
    nullptr
};

struct ValueCoefficients {
    float cosineSimilarity = 0, moduleSimilarity = 0, totalDistance = 1;
    IStateValue::CPtr srcValue, dstValue;
    ValueEvolution evolution = ValueEvolution::NONE;
};

class StateComparison {
public:

protected:
    unordered_map<IStateValue::CNew, ValueCoefficients> _valuesSimilarity;

public:
    IState::CNew _srcState, _dstState;
    IState::New _srcCleanState, _dstCleanState;
    StateComparison() {}
    StateComparison(const StateComparison & o) : _valuesSimilarity(o._valuesSimilarity) {}
    const IState::Ptr& sourceCleanState() const { return _srcCleanState; }
    const IState::Ptr& destinationCleanState() const { return _dstCleanState; }
    const IState::CPtr& sourceState() const { return _srcState; }
    const IState::CPtr& destinationState() const { return _dstState; }
    const unordered_map<IStateValue::CNew, ValueCoefficients>& valuesSimilarity() { return _valuesSimilarity; }

    float compareStates(const IState::CNew& stateSrc, const IState::CNew& stateDst, unordered_map<IStateValue::CNew, IStateValue::New> *pMapDiscovered = nullptr) {
        float percent = 0;
        IState::index_type s2Count = 0;
        IState::index_type sameKeyCount = 0; // Counts the number of keys that stateSrc has like stateDst

        _srcState = stateSrc;
        _dstState = stateDst;
        auto itState = stateDst->iterator();
        while (itState->hasNext()) {
            auto pair = itState->next();
            auto key = pair.first;
            string strKey = *key;
            ValueCoefficients& coeff = _valuesSimilarity[key];
            IStateValue::Ptr valueDst = pair.second;
            IStateValue::Ptr valueSrc;
            if (valueDst) {
                valueSrc = stateSrc->at(key);
                if (valueSrc) {
                    sameKeyCount++;
                    if (valueDst->size() == 1 && valueSrc->size() == 1) {
                        // Si son números únicos, hacer una comparación sencilla
                        float s1k = valueSrc->at(0);
                        float s2k = valueDst->at(0);
                        // 0 < are similar < are different < 1
                        float percInc = basicmath::floatSimilarity(s1k, s2k);
                        float mul = s1k * s2k;
                        coeff.cosineSimilarity = mul > 0 ? 1 : mul < 0 ? -1 : 0;
                        coeff.moduleSimilarity = 1.f - basicmath::floatSimilarityAbs(s1k, s2k);
                        coeff.totalDistance = percInc;
                        if (percInc > GOAP_FLT_EPSILON) {
                            coeff.evolution = (s2k > s1k) ? ValueEvolution::INC : ValueEvolution::DEC;
                        }
                        percent += percInc;
                    } else if (valueDst->size() >= 1 && valueSrc->size() >= 1) {
                        float thisModule, othersModule;
                        float cosDist = valueSrc->cosineDistance(valueDst, &thisModule, &othersModule);
                        float moduleDistance = 1.f - basicmath::floatSimilarity(thisModule, othersModule);
                        float percInc = std::max(0.f, 1.f - cosDist * moduleDistance);
                        coeff.cosineSimilarity = cosDist;
                        coeff.moduleSimilarity = moduleDistance;
                        coeff.totalDistance = percInc;
                        coeff.evolution = ValueEvolution::MODIFIED;
                        percent += percInc;
                    } else {
                        coeff.evolution = ValueEvolution::MODIFIED;
                        percent += 1.f;
                    }
                    if (pMapDiscovered) {
                        auto it = pMapDiscovered->find(key);
                        if (it == pMapDiscovered->end()) {
                            (*pMapDiscovered)[key] = valueDst;
                            switch  (coeff.evolution) {
                            case ValueEvolution::MODIFIED:
                                coeff.evolution = ValueEvolution::DISCOVERED;
                                break;
                            case ValueEvolution::INC:
                                coeff.evolution = ValueEvolution::DISCOVERED_INC;
                                break;
                            case ValueEvolution::DEC:
                                coeff.evolution = ValueEvolution::DISCOVERED_DEC;
                                break;
                            default:
                                break;
                            }
                        }
                    }
                    //LOG(DEBUG) << "Compared key=" << *key << ": ValueDst=" << *valueDst << "; ValueSrc=" << *valueSrc << ". AccPercent=" << percent;
                } else {
                    coeff.cosineSimilarity = 0;
                    coeff.moduleSimilarity = 0;
                    coeff.totalDistance = 1;
                    coeff.evolution = ValueEvolution::APPEARED;
                }
            }
            if (coeff.totalDistance > GOAP_FLT_EPSILON) {
                // There are changes in this value
                coeff.srcValue = valueSrc;
                coeff.dstValue = valueDst;
                if (valueSrc) {
                    _srcCleanState->put(key, valueSrc);
                }
                _dstCleanState->put(key, valueDst);
            }
            ++s2Count;
        }
        IState::index_type countDifferent = s2Count - sameKeyCount;
        float dist = (percent + countDifferent) / s2Count;
        //LOG(DEBUG) << "NumericDistance=" << dist << "\n  from " << *stateSrc << "\n    to " << *stateDst;
        return dist;
    }
};

struct ValueStats {
    IStateValue::CPtr maxVal, minVal;
    long long count = 0;
};

unordered_map<IStateValue::CPtr, ValueStats> getStateValueStats(const list<IState::CPtr>& lstComp, int percentThreshold = 50) {
    unordered_map<IStateValue::CPtr, ValueStats> ret;
    for (const auto& comp : lstComp) {
        auto it = comp->iterator();
        while(it->hasNext()) {
            auto pair = it->next();
            ValueStats &stat = ret[pair.first];
            ++stat.count;
            if (!stat.maxVal || *stat.maxVal < *pair.second) {
                stat.maxVal = pair.second;
            }
            if (!stat.minVal || *stat.minVal > *pair.second) {
                stat.minVal = pair.second;
            }
        }
    }
    int threshold = percentThreshold * ret.size() / 100;
    for (auto it = ret.begin(); it != ret.end(); ) {
        if (it->second.count < threshold) {
            it = ret.erase(it);
        } else {
            ++it;
        }
    }
    return ret;
}

TEST_F(GoapTest, TestStateComparison)
{
    IState::New state1 { {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP}, {"Credits", 10}, {"Uno", {1,2,0}}, {"Dos", {1,1,0}}, {"JustDisapeared", false}, {"EggIsOnBowl", false}, {"ButterIsOnBowl", false}, {"FlourIsOnBowl", false} };
    IState::New state2 { {"OwenTemperature", REF_TEMP + 3}, {"BowlTemperature", REF_TEMP}, {"Credits", 2}, {"Uno", {5,10,0}}, {"Dos", {0,1,0}}, {"JustCreated", true}, {"EggIsOnBowl", false}, {"ButterIsOnBowl", false}, {"FlourIsOnBowl", false} };
    StateComparison comp;
    float dist1 = comp.compareStates(state1, state2);
    auto &mapSim = comp.valuesSimilarity();
    for (auto & it : mapSim) {
        LOG(INFO) << "State:" << *it.first << ",\n    cosDistance:" << it.second.cosineSimilarity << ",    moduleDistance:" << it.second.moduleSimilarity << ",    percInc:" << it.second.totalDistance;
    }
    LOG(INFO) << "Clean Src State: " << *comp._srcCleanState;
    LOG(INFO) << "Clean Dst State: " << *comp._dstCleanState;
    ASSERT_EQ(nullptr, comp._srcCleanState->at("EggIsOnBowl"));
    ASSERT_EQ(nullptr, comp._dstCleanState->at("EggIsOnBowl"));

    auto& uno = mapSim.at("Uno");
    ASSERT_FLOAT_EQ(1, uno.cosineSimilarity);
    ASSERT_FLOAT_EQ(0.2f, uno.moduleSimilarity);
    ASSERT_FLOAT_EQ(0.8f, uno.totalDistance);
}

class ClassifyAndSearch {
protected:
    IPlanner::Ptr _planner; // = backing.planner();
    map<string, long long> _mapActionAcepted;
    map<string, list<IState::CPtr> > _mapActionToTriggerSrcState;
    unordered_map<IStateValue::CNew, map<ValueEvolution, map<string, list<ValueCoefficients> > > > _mapValueToAction;
    unordered_map<string, map<IStateValue::CNew, map<ValueEvolution, list<ValueCoefficients> > > > _mapActionToValue;
    unordered_map<IStateValue::CNew, IStateValue::New> _mapDiscoveredValues;

public:
    ClassifyAndSearch(const IPlanner::Ptr& planner) : _planner(planner) {
        _planner->actionStateFunction([&](const IPlanningAction::CPtr &action, const IState::CPtr &initialState, IState::CPtr nextState) {
            if (nextState) {
                const string& actionName = *action->name();
                ++_mapActionAcepted[actionName];
                StateComparison comp;
                float dist1 = comp.compareStates(initialState, nextState , &_mapDiscoveredValues); //
                //auto &mapSim = comp.valuesSimilarity();
                //_mapActionToSrcToDstState[actionName][comp.sourceCleanState()][comp.destinationCleanState()] = comp;
                if (nextState) {
                    _mapActionToTriggerSrcState[actionName].push_back(initialState);
                }
                auto itValue = comp.destinationCleanState()->iterator();
                const IState::Ptr& srcState = comp.sourceCleanState();
                while (itValue->hasNext()) {
                    auto pair = itValue->next();
                    auto &key = pair.first;
                    auto &valSim = comp.valuesSimilarity().at(key);
                    _mapValueToAction[key][valSim.evolution][actionName].push_back(valSim);
                    _mapActionToValue[actionName][key][valSim.evolution].push_back(valSim);
                }
            }
        });
    }

    /**
     * @brief backtrack Find an action secuence from the targetState up to the initialState
     * @param initialState
     * @param targetState
     */
    void backtrack(const IState::CNew &initialState, const IState::CNew &targetState) {
        auto itTargetValues = targetState->iterator();
        while (itTargetValues->hasNext()) {
            auto pair = itTargetValues->next();
            const IStateValue::CPtr &key = pair.first;
            const IStateValue::CPtr &value = pair.second;
            // Find the needed states to trigger the action causing this value
            auto itValue = _mapValueToAction.find(key);
            if (itValue != _mapValueToAction.end()) {
                list<string> lstStrAction;
                for (ValueEvolution evol : {ValueEvolution::APPEARED, ValueEvolution::DISCOVERED, ValueEvolution::DISCOVERED_INC, ValueEvolution::DISCOVERED_DEC}) {
                    auto itEvol = itValue->second.find(evol);
                    if (itEvol != itValue->second.end()) {
                        for(auto itAction : itEvol->second) {
                            lstStrAction.push_back(itAction.first);
                        }
                    }
                }
                for (string &actionName : lstStrAction) {
                    _mapActionAcepted[actionName];
                }
                // auto itEvol = itValue->second.find(ValueEvolution::APPEARED);
                // if (itEvol != itValue->second.end()
                //         || (itEvol = itValue->second.find(ValueEvolution::DISCOVERED)) != itValue->second.end()
                //         || (itEvol = itValue->second.find(ValueEvolution::DISCOVERED_INC)) != itValue->second.end()
                //         || (itEvol = itValue->second.find(ValueEvolution::DISCOVERED_INC)) != itValue->second.end() ) {
                //     for(auto itAction : itEvol->second) {
                //         lstStrAction.push_back(itAction.first);
                //     }
                // }
            }
        }
    }

    void log() {
        for (auto& it : _mapActionAcepted) {
            LOG(INFO) << "Action: \"" << it.first << ", Count: " << it.second;
        }
        for (auto& it1 : _mapValueToAction) {
            for (auto& it2 : it1.second) {
                auto szEvol = ValueEvolution2Name[(int)it2.first];
                for (auto& it3 : it2.second) {
                    auto& comp = it3.second;
                    if(it2.first == ValueEvolution::DEC) {
                        auto nElems = it1.second[ValueEvolution::INC][it3.first].size();
                        if (nElems > 5 && comp.size() > 5) {
                            continue;
                        }
                    } else if(it2.first == ValueEvolution::INC) {
                        auto nElems = it1.second[ValueEvolution::DEC][it3.first].size();
                        if (nElems > 5 && comp.size() > 5) {
                            continue;
                        }
                    }
                    if (comp.size() == 0) {
                        continue;
                    }
                    LOG(INFO) << "Value: " << *it1.first <<", ValueEvolution: " << szEvol <<  ", Action: " << it3.first << ", #" << comp.size();
                }
            }
        }

        typedef pair<IStateValue::CPtr, ValueStats> p_type;
        vector<p_type> lstStats;
        for (auto it : _mapActionToTriggerSrcState) {
            const string & actionName = it.first;
            unordered_map<IStateValue::CPtr, ValueStats> mapStats = getStateValueStats(it.second, 50);
            lstStats.clear();
            for (auto it : mapStats) {
                lstStats.push_back(make_pair(it.first, it.second));
            }
            sort(lstStats.begin(), lstStats.end(), [](const p_type& a, const p_type& b) {
                return a.second.count > b.second.count;
            });
            for (auto it : lstStats) {
                LOG(INFO) << "Stats for Action: " << actionName << ", Count: " << it.second.count << ", Value: " << *it.first << ", Max: " << *it.second.maxVal << ", Min: " << *it.second.minVal;
            }
        }
        for (auto it1 : _mapActionToValue) {
            for (auto& it2 : it1.second) {
                for (auto& it3 : it2.second) {
                    auto& comp = it3.second;
                    auto szEvol = ValueEvolution2Name[(int)it3.first];
                    LOG(INFO) << "Action: " << it1.first << ", Value: " << *it2.first << ", ValueEvolution: " << szEvol <<", #" << comp.size();
                }
            }
        }
    }
};

TEST_F(GoapTest, TestBackingAPie)
{
    backing_a_pie backing;
    ClassifyAndSearch classify(backing.planner());
    float REF_TEMP = backing_a_pie::REF_TEMP;
    backing.backing_plan({ {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP}, {"Credits", 10} }, {{"PieIsReadyForEat", 1}});
    IState::CPtr initialState = backing.initialState();
    list<IPlanningAction::CPtr> plan = backing.MakePlan();
    classify.log();

    //for (auto& it1 : mapActionToSrcToDstState) {
    //    for (auto& it2 : it1.second) {
    //        for (auto& it3 : it2.second) {
    //            auto& comp = it3.second;
    //            LOG(INFO) << "Action resume: \"" << it1.first << "\", {" << *it2.first << "}, {" << *it3.first << "}";
    //            for (auto & it : it3.second.valuesSimilarity()) {
    //                if (it.second.totalDistance > GOAP_FLT_EPSILON)
    //                LOG(INFO) << "Value:" << *it.first << ",\n    cosineSimilarity:" << it.second.cosineSimilarity << ",    moduleSimilarity:" << it.second.moduleSimilarity << ",    totalDistance:" << it.second.totalDistance;
    //            }
    //        }
    //        int a = 0;
    //    }
    //}

}

