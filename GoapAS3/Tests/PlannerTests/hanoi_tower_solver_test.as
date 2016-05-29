package Tests.PlannerTests
{
	import flash.utils.Dictionary;
	import flash.utils.getTimer;
	
	import Goap.IReadState;
	import Goap.IWriteState;
	import Goap.Planner;
	import Goap.PlanningAction;
	import Goap.State;
	import Goap.Planning.IPlanningAction;
	import Goap.Planning.Comparer.ExactStateComparer;
	import Goap.Planning.Meter.FunctionStateMeter;
	import Goap.Planning.Meter.PlanningStateMeter;

	/**
	 * Plan Formulation
	 * 
	 * A character generates a plan in real-time by supplying some goal to satisfy to a system 
	 * called a planner. The planner searches the space of actions for a sequence that will take 
	 * the character from his starting state to his goal state. This process is referred to as 
	 * formulatinga plan. If the planner is successful, it returns a plan for the character to follow 
	 * to direct his behavior. The character follows this plan to completion, invalidation, or until 
	 * another goal becomes more relevant. If another goal activates, or the plan in-progress 
	 * becomes invalid for any reason, the character aborts the current plan and formulates a 
	 * new one. 
	 */
	public class hanoi_tower_solver_test
	{
		
		private function tower_plan(initial:Object, goal:Object, n:int = 3):void
		{
			_n = n;
			_initialState = new State (initial);
			_goalState = new State (goal);
			_planner = _planners[n] as Planner;
			if (!_planner) {
				var planningActions:Vector.<IPlanningAction> = new <IPlanningAction>
				[
					new PlanningAction(
						"Move from A to B",
						function(state:IReadState):Boolean { return validate(state, "A", "B", n); },
						function(state:IWriteState):void   {            move(state, "A", "B", n); }),
					new PlanningAction(
						"Move from A to C",
						function(state:IReadState):Boolean { return validate(state, "A", "C", n); },
						function(state:IWriteState):void   {            move(state, "A", "C", n); }),
					new PlanningAction(
						"Move from B to A",
						function(state:IReadState):Boolean { return validate(state, "B", "A", n); },
						function(state:IWriteState):void   {            move(state, "B", "A", n); }),
					new PlanningAction(
						"Move from B to C",
						function(state:IReadState):Boolean { return validate(state, "B", "C", n); },
						function(state:IWriteState):void   {            move(state, "B", "C", n); }),
					new PlanningAction(
						"Move from C to A",
						function(state:IReadState):Boolean { return validate(state, "C", "A", n); },
						function(state:IWriteState):void   {            move(state, "C", "A", n); }),
					new PlanningAction(
						"Move from C to B",
						function(state:IReadState):Boolean { return validate(state, "C", "B", n); },
						function(state:IWriteState):void   {            move(state, "C", "B", n); })
				];
				_planner = new Planner(Planner.BreadthFirst, planningActions);
				_planners[n] = _planner;
			}
		}

		private var _n:int;
		private var _planningStateMeter:PlanningStateMeter;
		public function MakePlan():Vector.<IPlanningAction>
		{
			if (!_planningStateMeter || _planningStateMeter.goalState != _goalState) {
				var functionStateMeter:FunctionStateMeter = new FunctionStateMeter(_goalState);
				functionStateMeter.monotonic = true;
				functionStateMeter.fnDistance = function (state:IReadState, stateMeter:FunctionStateMeter):Number 
				{
					var ret:Number = 1.0;
					ret = stateMeter.exactStateMeter.distance(state); // Distance to goal
					var distance:Number;
					if (state.at(concatStringInt("A", _n)) == _n) {
						// A conditional suggestion: First remove all elements from the tower 'A'
						distance = ExactStateComparer.singleton.distance(state, _initialState);
						distance = 1.0 - distance / (2 * _n + 1.0);
						if (ret > distance)
							ret = distance;
					} 
					return ret;
				};
				functionStateMeter.fnEnough = function (state:IReadState, stateMeter:FunctionStateMeter):Boolean 
				{
					return stateMeter.exactStateMeter.enough(state);
				};
				_planningStateMeter = functionStateMeter;
			}
			var t1:int = getTimer();
			var actions:Vector.<IPlanningAction> = _planner.makePlanCached(_initialState, _planningStateMeter);
			var t2:int = getTimer();
			trace("[Time]", _goalState.length, "deep,", (Math.pow(2, _goalState.length)-1), "optimus,", actions.length + " actions, "  + (t2-t1) + "ms")
			return actions;
		}
		
		public function run2():void
		{
			var plan:Vector.<IPlanningAction>;
			tower_plan( { A1:1, A2:2, A3:3, C1:null, C2:null, C3:null }, { C1:1, C2:2, C3:3 }, 3);
			plan = MakePlan();
			
			tower_plan( { A1:1, A2:2, A3:3, A4:4, A5:5, A6:6, A7:7, A8:8 }, { C1:1, C2:2, C3:3, C4:4, C5:5, C6:6, C7:7, C8:8 }, 8);
			plan = MakePlan();
		}
		
		public function run():void
		{	
			var plan:Vector.<IPlanningAction>;
			tower_plan( { A1:1, A2:2, A3:3 }, { C1:1, C2:2, C3:3 }, 3);
			plan = MakePlan();
			
			tower_plan( { A1:1, A2:2, A3:3, A4:4 }, { C1:1, C2:2, C3:3, C4:4 }, 4);
			plan = MakePlan();
			//trace ("Plan 4:\n" + Planner.PlanToString(plan, initialState));
			tower_plan( { A1:1, A2:2, A3:3, A4:4, A5:5 }, { C1:1, C2:2, C3:3, C4:4, C5:5 }, 5);
			plan = MakePlan();
			
			tower_plan( { A1:1, A2:2, A3:3, A4:4, A5:5, A6:6 }, { C1:1, C2:2, C3:3, C4:4, C5:5, C6:6 }, 6);
			plan = MakePlan();
			
			tower_plan( { A1:1, A2:2, A3:3, A4:4, A5:5, A6:6, A7:7 }, { C1:1, C2:2, C3:3, C4:4, C5:5, C6:6, C7:7 }, 7);
			plan = MakePlan();
			
			tower_plan( { A1:1, A2:2, A3:3, A4:4, A5:5, A6:6, A7:7, A8:8 }, { C1:1, C2:2, C3:3, C4:4, C5:5, C6:6, C7:7, C8:8 }, 8);
			plan = MakePlan();
			
			tower_plan( { A3:1, A4:2, A5:3, A6:4, A7:5, B6:6, B7:7 }, { C1:1, C2:2, C3:3, C4:4, C5:5, C6:6, C7:7 }, 7);
			plan = MakePlan();
			
			//trace ("Plan 5:\n" + Planner.PlanToString(plan, initialState));
		}
		
		// A1 1  B1 |  C1 |
		// A2 2  B2 |  C2 |
		// A3 3  B3 |  C3 |
		//   / \   / \   / \
		
		private var _planner:Planner;
		private var _planners:Dictionary = new Dictionary;
		private var _initialState:IReadState; 
		private var _goalState:IReadState;
		
		//private var _stringBuffer:StringBuffer = new StringBuffer;
		/**
		 * Test if an action is posible given a state
		 * Validates that the state can change in the way it is specified in the 'from' 'to' strnigs.
		 * @param n The max tower height.
		 */
		[Inline]
		private static function validate(state:IReadState, from:String, to:String, n:int = 3):Boolean
		{
			var i:int;
			var val:* = null;
			var onFromTop:int = 0;
			for (i = 1; i <= n; ++i) {
				val = state.at(concatStringInt(from, i));
				if (val != null)
					break;
			}
			if (!(val is int))
				return false;
			
			onFromTop = val as int;
		
			var onToTop:int = 0;
			var trigerTo:Boolean = false;
			val = null;
			for (i = 1; i <= n; ++i) {
				val = state.at(concatStringInt(to, i));
				if (val != null)
					break;
			}
			if (val is int) {
				trigerTo = true;
				onToTop = val as int;
			}
			return !trigerTo || onToTop > onFromTop;
		}
		
		private static var _strCache:Object = { };
		/**
		 * Helper function to cache strings in the form: 'String' + 'int'
		 * This is a way of lowering pression from the GC.
		 * The integger must be a low number.
		 */
		private static function concatStringInt(str:String, n:int):String
		{
			var arr:Array = _strCache[str];
			if (arr == null) {
				arr = [];
				_strCache[str] = arr;
			}
			for (var i:int = arr.length; i <= n; ++i) {
				arr.push(str+i);
			}
			return arr[n];
		}
		
		// The 'executor' function
		[Inline]
		private static function move(state:IWriteState, from:String, to:String, n:int = 3):void
		{
			var i:int;
			var val:* = null;
			var a:String;
			var onFromTop:int;
			for (i = 1; i <= n; ++i) {
				var astr:String = concatStringInt(from, i);
				val = state.at(astr);
				if (val != null) {
					a = astr;
					break;
				}
			}
			if (a == null) {
				throw new Error("Can't trig from '" + from + "' to '" + to + "'");
			}
			var b:String;
			var onToTop:int;
			var trigerTo:Boolean = false;
			for (i = 1; i <= n; ++i) {
				val = state.at(concatStringInt(to, i));
				if (val != null) {
					if (i-- == 1)
						throw new Error("Can't put to '" + to + "'");
					else
						b = concatStringInt(to, i);
					trigerTo = true;
					break;
				}
			}
			if (!trigerTo) {
				b = concatStringInt(to, n);
			}
			// Update the source state
			state.put(b, state.at(a));
			state.remove(a);
		}
	}	
}
