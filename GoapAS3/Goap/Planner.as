package Goap
{
	import flash.utils.Dictionary;
	import flash.utils.getTimer;
	
	import Goap.Graph.Path;
	import Goap.Planning.IPlanner;
	import Goap.Planning.IPlanningAction;
	import Goap.Planning.Comparer.NumericStateComparer;
	import Goap.Planning.Meter.IPlanningStateMeter;
	import Goap.PrioritizedCollections.IPrioritized;
	import Goap.PrioritizedCollections.PrioritizedQueue;
	import Goap.PrioritizedCollections.PrioritizedStack;

	/**
	 * Plan
	 * 
	 * The plan is simply the name for a sequence of actions. A plan that satisfies a goal refers 
	 * to the valid sequence of actions that will take a character from some starting state to some 
	 * state that satisfies the goal.
	 * This class holds a cache of precomputed paths for different goal states using the same set of
	 * planning actions.
	 */
	public class Planner implements IPlanner
	{
		public static const BreadthFirst:int = 1; // Queue: FIFO
		public static const DepthFirst:int = 2;   // Stack: LIFO		
		private var _planningMethod:int = BreadthFirst;
		
		/**
		 * The set of actions that can be taken.
		 * Every action has a precondition to be met in order to be executed.
		 * An action execution implies a state change.
		 * These are the rules of the system.
		 */
		private var _planningActions:Vector.<IPlanningAction>;
		
		public function Planner(planningMethod:int, 
								planningActions:Vector.<IPlanningAction>)
		{
			_planningMethod = planningMethod;
			_planningActions = planningActions;
		}
		
		[Inline]
		public final function get planningActions():Vector.<IPlanningAction>
		{
			return _planningActions;
		}

		[Inline]
		public final function get planningMethod():int
		{
			return _planningMethod;
		}

		public static function planToString(actionsArray:Vector.<IPlanningAction>, initialState:IReadState = null):String
		{
			var totalCost:Number = 0;
			var ret:String;
			var action:IPlanningAction;
			if (!actionsArray) {
				ret = '"No plan"\n';
			} else {
				var reachedByPath:IReadState = initialState;
				if (reachedByPath) {
					totalCost += reachedByPath.getCost();
					ret = "0.\"init\" totalCost=" + totalCost + " state=" + reachedByPath + "\n";
				}
				for (var i:int = 0; i < actionsArray.length; ++i) {
					action = actionsArray[i];
					if (action == null)
						break;
					ret += (i+1) + ".\"" + action + "\" ";
					if (reachedByPath) {
						reachedByPath = action.execute(reachedByPath); // Advance the state
						totalCost += reachedByPath.getCost();
						ret += "totalCost=" + totalCost + " state=" + reachedByPath + "\n";
					}
				}
			}
			return ret;
		}
		
		/**
		 * Helper function to compute the reached state resulting of executing all the allowed actions from 
		 * the initial state.
		 * The cost of the resulting state is overwritten with the sum of all intermediate costs.
		 */
		public static function executeActions(actions:Vector.<IPlanningAction>, initialState:IReadState):IWriteState
		{
			var nextState:IWriteState;
			var initial:IWriteState = initialState.clone();
			var reachedByPath:IWriteState = initial;
			reachedByPath.cost = initialState.getCost();
			try {
				var accCost:Number = reachedByPath.cost;
				for each(var action:IPlanningAction in actions) {
					if (!action.canExecute(reachedByPath))
						break; // Do not continue
					nextState = action.execute(reachedByPath); // Advance the state
					reachedByPath.dispose();
					reachedByPath = nextState;
					accCost += reachedByPath.cost;
				}
				reachedByPath.cost = accCost;
			} catch (e:Error) {
				trace ("[Planner] Error: Can't executeActions().", e.getStackTrace());
			}
			return reachedByPath;
		}
		
		
		/**
		 * Returns the plan: A Vector of IPlanningAction.
		 * If no plan is found a null is returned.
		 */
		public function makePlan(initialState:IReadState, 
								 planningStateMeter:IPlanningStateMeter, 
								 actionsArray:Vector.<IPlanningAction> = null, 
								 states:Vector.<IReadState> = null):Vector.<IPlanningAction>
		{
			var i:int;
			const isMonotonic:Boolean = planningStateMeter.monotonic; 
			var minDistance:Number = 1;
			var analyzedPaths:int = 0;
			var rejectedPaths:int = 0;
			var pathCost:Number = 0;
			var action:IPlanningAction;
			var reachedByPath:IReadState;
			var unvisitedPathes:IPrioritized = this.unvisitedPathes;
			var visitedStates:StatesDictionary = new StatesDictionary;
			visitedStates.setAt(initialState, initialState);
			
			var gather:Function = function (pathCost:Number, minDistance:Number):Number {
				var plan:Path = null;
				var min:Number = minDistance;
				for each(action in _planningActions) {
					try {
						if (action.canExecute(reachedByPath)) {
							var nextState:IWriteState = action.execute(reachedByPath);
							var distance:Number = planningStateMeter.distance(nextState);
							// Adds a new path leave
							if (isMonotonic && min > distance) {
								// When monotonic Meters are used, a decreasing in the distance to the goal
								// found in a path increases the preference to explore that path.
								minDistance = distance;
								distance *= nextState.cost;
								// Use a very low cost to give more chances for this path to be analyzed
								distance = 4*distance - pathCost - 10; // Landmark: Put the distance betwen -10 and -6
							} else {
								distance *= nextState.cost;
							}
							plan = Path.getNew(action, path, pathCost + distance);
							// Add the path to be analyzed after all the paths of this priority level be processed
							unvisitedPathes.pushLazy(plan);
							nextState.dispose();
						}
					} catch (e:Error) {
						trace("[Planner] Error: ", e.getStackTrace());
					}
				}
				if (plan == null && path != null) {
					// The path has no children so it will not be used anymore
					path.dispose();
				}
				return minDistance;
			};
			
			// Gather all the actions that can be reached from the intial state
			reachedByPath = initialState;
			minDistance = gather(0, minDistance);

			if (unvisitedPathes.isEmpty) {
				trace("[MakePlan] There are no paths from the initial state!!!");
			}
			var path:Path;
			var bReachedGoal:Boolean = false;
			// Search all the path forks neccesary to reach the goalState
			while (!unvisitedPathes.isEmpty) {
				// Takes the path with the lower cost available
				path = unvisitedPathes.pop();
				reachedByPath = path.executeFromRoot(initialState);
				// If this state was visited in other paths, do not compute it again
				var visitedState:IReadState = visitedStates.at(reachedByPath);
				if (visitedState != null) {
					path.dispose();
					path = null;
					++rejectedPaths;
					continue;
				}
				// If this path takes us to the goal state, then return it
				bReachedGoal = planningStateMeter.enough(reachedByPath);
				if (bReachedGoal) {
					actionsArray = path.getActions(actionsArray); // Get the planning actions from the intial state
					if (states)
						path.getStates(initialState, states);
					path.dispose();
					break;
				}
				// Mark the state as visited so we don't reanalyse it while searching other paths
				visitedStates.setAt(reachedByPath, reachedByPath);
				// Continue gathering all the actions that can be reached from current state
				minDistance = gather(path.cost, minDistance);
				++analyzedPaths;
			}
			if (bReachedGoal) {
				trace("[MakePlan] OK. analyzedPaths:", analyzedPaths, ", rejectedPaths:", rejectedPaths,
					", unvisitedPathes:", unvisitedPathes.length);
			} else {
				trace("[MakePlan] FAIL. analyzedPaths:", analyzedPaths, ", rejectedPaths:", rejectedPaths,
					", unvisitedPaths:", unvisitedPathes.length);
				actionsArray = null;
			}
			return actionsArray;
		}
		
		/**
		 * A version of makePlan() with the help of cached plans.
		 * @note The found cached plan is not warranted to use the supplied planningStateMeter.
		 */
		public function makePlanCached(initialState:IReadState, 
								 planningStateMeter:IPlanningStateMeter, 
								 actionsArray:Vector.<IPlanningAction> = null, 
								 states:Vector.<IReadState> = null):Vector.<IPlanningAction>
		{
			var goalState:IWriteState = planningStateMeter.goalState.clone();
			actionsArray = findPlan(initialState, goalState, actionsArray);
			if (actionsArray.length == 0) {
				if (!states) {
					states = new Vector.<IReadState>;
				}
				actionsArray = makePlan(initialState, planningStateMeter, actionsArray, states);
				if (actionsArray != null) {
					// Substitute the reached state for a clean goalState
					var cost:Number = states[states.length - 1].cost;
					goalState.cost = cost;
					states[states.length - 1] = goalState;
					states.unshift(initialState); // Add the initial state to the list
					cacheStates(states, actionsArray);
				}
			}
			return actionsArray;
		}

		private function get unvisitedPathes():IPrioritized
		{
			var prioritized:IPrioritized = null;
			switch (planningMethod)
			{
				case BreadthFirst:
					prioritized = new PrioritizedQueue();
					break;
				case DepthFirst:
					prioritized = new PrioritizedStack();
					break;
			}
			return prioritized;
		}
		
		private var _statesCaches:Dictionary = new Dictionary;
		
		/**
		 * Caches the list of states and their planning.
		 * The 'states' vector must have one more element than 'plan'.
		 * The last element of 'states' is its goal.
		 * This function does not tries to reuse already cached 'states' and 'plan' so it
		 * should be used after findPlanning() returned a zero length vector.
		 */
		public function cacheStates(states:Vector.<IReadState>, plan:Vector.<IPlanningAction>):void
		{
			var goalState:IReadState = states[states.length -1];
			var plannings:Vector.<StatesPlan> = _statesCaches[goalState.hash];
			if (plannings == null) {
				plannings = new Vector.<StatesPlan>;
				_statesCaches[goalState.hash] = plannings;
			}
			var statePlan:StatesPlan = new StatesPlan;
			statePlan.states = states;
			statePlan.plan = plan;
			plannings.push(statePlan);
		}
		
		/**
		 * Finds a plan in the cached data. 
		 * If the returning vector has a length of 0 elements, no plan has been found in cache.
		 */
		public function findPlan(intialState:IReadState,
								 goalState:IReadState,
								 plan:Vector.<IPlanningAction> = null):Vector.<IPlanningAction>
		{
			var retPlan:Vector.<IPlanningAction>;
			var nStates:int;
			var nState:int;
			var i:int;
			var offset:int = -1;
			var nearStatePlan:StatesPlan = null;
			var nearDistance:Number = 1.0;
			var nearOffset:int = -1;
			if (plan != null) {
				plan.length = 0;
				retPlan = plan;
			} else {
				retPlan = new Vector.<IPlanningAction>;
			}
			// Search a cached plan for this goal
			var plannings:Vector.<StatesPlan> = _statesCaches[goalState.hash];
			if (plannings != null) {
				for each(var statePlan:StatesPlan in plannings) {
					nStates = statePlan.states.length -1;						
					for (nState = 0; nState < nStates; nState++ ) {
						var storedState:IReadState = statePlan.states[nState];
						var distance:Number = NumericStateComparer.singleton.distance(storedState, intialState);
						if (nearDistance > distance) {
							nearDistance = distance;
							nearStatePlan = statePlan;
							nearOffset = nState;
							if (nearDistance <= 0.0) {
								break;
							}
						}
					}
					if (nearDistance <= 0.0) {
						break;
					}
				}
				if (nearOffset >= 0) {
					nStates = nearStatePlan.plan.length;
					retPlan.length = nStates - nearOffset;
					// Copy the necessary portion of actions array
					i = 0;
					for (nState = nearOffset; nState < nStates; nState++ ) { 
						retPlan[i++] = nearStatePlan.plan[nState];
					}
					// Test if the found plan is valid
					var reachedState:IReadState = executeActions(retPlan, intialState);
					var bEnough:Boolean = NumericStateComparer.singleton.enough(reachedState, goalState);
					if (!bEnough) {
						// This plan is not good enough
						retPlan.length = 0;
					}
				}
			}
			return retPlan;
		}
	}	
}
