package Tests.PlannerTests
{
	import flash.utils.getTimer;
	
	import Goap.IReadState;
	import Goap.IWriteState;
	import Goap.Planner;
	import Goap.PlanningAction;
	import Goap.State;
	import Goap.Planning.IPlanningAction;
	import Goap.Planning.Comparer.NumericStateComparer;
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
	public class backing_a_pie
	{
		public static const REF_TEMP:int = 300;
		private var _planner:Planner;
		private var _initialState:IReadState; 
		private var _goalState:IReadState;
		
		public function backing_a_pie()
		{
			backing_actions();
		}
		
		private static function wait(state:IWriteState):IWriteState
		{
			var finalTemperature:int;
			var OwenTemperature:int = state.at("OwenTemperature");
			if (state.at("OwenIsOn") == true) {
				if (OwenTemperature < REF_TEMP + 200)
					OwenTemperature += 40;
			} else if (OwenTemperature > REF_TEMP) {
				OwenTemperature -= 20;
			}
			var BowlTemperature:int = state.at("BowlTemperature");
			if (state.at("BowlLocation") == "Owen") {
				if (OwenTemperature > BowlTemperature + 20)
					BowlTemperature += 20;
				else if (OwenTemperature <= BowlTemperature - 10)
					BowlTemperature -= 10;
			}else if (BowlTemperature > REF_TEMP) {
				BowlTemperature -= 20;
			}

			state.put("BowlTemperature", BowlTemperature);
			state.put("OwenTemperature", OwenTemperature);
			
			if (state.at("PieIsComing") > 0) {
				state.add("PieIsComing", 1);
			}
			return state;
		}
		
		private function backing_plan(initial:Object, goal:Object):void
		{
			_initialState = State.getNew (initial);
			_goalState = State.getNew (goal);
		}
		
		/**
			Baking a pie
			============
			If the overall goal is to bake a pie the end-state could be
			(PieIsBaked, true). The plan is then formulated based on the actions
			available and their costs, which for instance might include TurnOnOwen,
			PutBowlOnTable, AddEggToBowl, AddButterToBowl, AddFlourToBowl,
			MixBowlIngredients, PutBowlInOwen, Wait, and PutBowlOnTable. In this
			case the first action would be TurnOnOwen which has no preconditions, just as
			PutBowlOnTable. However the actions AddEggToBowl, AddButterToBowl
			and AddFlourToBowl might have the precondition (BowlIsOnTable, true)
			which is the very effect of the action PutBowlOnTable. If the mixture is
			supposed to fare better if the egg is used before that butter, which in turn is
			used before the flour; then AddEggToBowl is given the lowest cost value of the
			three actions whilst AddButter is given a lower cost value than
			AddFlourToBowl. The baking is then allowed to continue by utilizing the
			remaining actions of the plan, resulting in a baked pie once the final action
			PutBowlOnTable has been executed.
		 */
		private function backing_actions():void
		{
			var planningActions:Vector.<IPlanningAction> = new <IPlanningAction>
			[
				new PlanningAction(
					"TurnOnOwen",
					function(state:IReadState):Boolean { return state.at("OwenIsOn") != true; },
					function(state:IWriteState):void   { wait(state.put("OwenIsOn", true)); }),
				new PlanningAction(
					"TurnOffOwen",
					function(state:IReadState):Boolean { return state.at("OwenIsOn") == true; },
					function(state:IWriteState):void   { wait(state.put("OwenIsOn", false)); }),
				new PlanningAction(
					"PutBowlOnTable",
					function(state:IReadState):Boolean { return state.at("BowlLocation") != "Table"; },
					function(state:IWriteState):void   { wait(state.put("BowlLocation", "Table")); }),
				new PlanningAction(
					"AddEggToBowl",
					function(state:IReadState):Boolean { return state.at("BowlLocation") == "Table" && state.at("EggIsOnBowl") != true; },
					function(state:IWriteState):void   { wait(state.put("EggIsOnBowl", true).mulCost(state.at("ButterIsOnBowl") != true ? 0.9 : 1.0)); }),
				new PlanningAction(
					"AddButterToBowl",
					function(state:IReadState):Boolean { return state.at("BowlLocation") == "Table" && state.at("ButterIsOnBowl") != true; },
					function(state:IWriteState):void   { wait(state.put("ButterIsOnBowl", true).mulCost(state.at("FlourIsOnBowl") != true ? 0.9 : 1.0)); }),
				new PlanningAction(
					"AddFlourToBowl",
					function(state:IReadState):Boolean { return state.at("BowlLocation") == "Table" && state.at("FlourIsOnBowl") != true; },
					function(state:IWriteState):void   { wait(state.put("FlourIsOnBowl", true)); }),
				new PlanningAction(
					"MixBowlIngredients",
					function(state:IReadState):Boolean { return state.at("BowlLocation") == "Table" && /*state.at("IngredientsAreMixed") != true &&*/ state.at("EggIsOnBowl") == true && state.at("ButterIsOnBowl") == true && state.at("FlourIsOnBowl") == true; },
					function(state:IWriteState):void   { wait(state.put("IngredientsAreMixed", true)); }),
				new PlanningAction(
					"PutBowlInOwen",
					function(state:IReadState):Boolean { return state.at("BowlLocation") != "Owen" && state.at("IngredientsAreMixed") == true && state.at("OwenTemperature") >= (REF_TEMP+100); },
					function(state:IWriteState):void   { wait(state.put("BowlLocation", "Owen")); }),
				new PlanningAction(
					"BakePie",
					function(state:IReadState):Boolean { return state.at("BowlLocation") == "Owen" && /*state.at("PieIsBaked") != true &&*/ state.at("BowlTemperature") >= (REF_TEMP+100); },
					function(state:IWriteState):void   { wait(state.put("PieIsBaked", true)); }),
				new PlanningAction(
					"PieReadyForEat",
					function(state:IReadState):Boolean { return state.at("PieIsBaked") == true && state.at("BowlLocation") == "Table" && state.at("BowlTemperature") < (REF_TEMP+30); }, 
					function(state:IWriteState):void   { wait(state.put("PieIsReadyForEat", true)); }),
				new PlanningAction(
					"OrderPie",
					function(state:IReadState):Boolean { return state.at("Credits") >= 8; }, 
					function(state:IWriteState):void   { wait(state.put("PieIsComing", 1).add("Credits", -8).mulCost(13)); }),
				new PlanningAction(
					"ReceivePie",
					function(state:IReadState):Boolean { return state.at("PieIsComing") >= 5; }, 
					function(state:IWriteState):void   { wait(state.put("PieIsReadyForEat", true).put("PieIsComing", 0)); }),
				new PlanningAction(
					"Wait",
					function(state:IReadState):Boolean { return true; },
					function(state:IWriteState):void   { wait(state.mulCost(0.9)); } )
			];
			_planner = new Planner(Planner.BreadthFirst, planningActions);
		}

		private var _backingHelper:IReadState = State.getNew({EggIsOnBowl:true, ButterIsOnBowl:true, FlourIsOnBowl:true, IngredientsAreMixed:true, PieIsBaked:true});
		private var _orderHelper:IReadState = State.getNew({PieIsComing:5});
		private var _planningStateMeter:PlanningStateMeter;
		public function MakePlan():Vector.<IPlanningAction>
		{
			if (!_planningStateMeter || !_planningStateMeter.goalState.equals(_goalState)) {
				var functionStateMeter:FunctionStateMeter = new FunctionStateMeter(_goalState);
				functionStateMeter.fnDistance = function (state:IReadState, stateMeter:FunctionStateMeter):Number 
				{
					var ret:Number = 1.0;
					ret = stateMeter.numericStateMeter.distance(state); // Distance to goal
					var distance:Number = ret;
					if (state.at("PieIsComing") == 0) {
						// A conditional suggestion
						distance = NumericStateComparer.singleton.distance(state, _backingHelper) * 0.8 + 0.2;
					}
					if (state.at("OwenTemperature") == REF_TEMP ) {
						// A conditional suggestion
						distance = NumericStateComparer.singleton.distance(state, _orderHelper) * 0.8 + 0.2;
					}
					if (ret > distance)
						ret = distance;
					return ret;
				};
				_planningStateMeter = functionStateMeter;
			}

			var actionsArray:Vector.<IPlanningAction>;
			var t1:int = getTimer();
			actionsArray = _planner.makePlanCached(_initialState, _planningStateMeter);
			var t2:int = getTimer();
			trace("[Time]", actionsArray.length, "actions,", (t2-t1) + "ms")
			return actionsArray;
		}
		
		public function run():void
		{
			var plan:Vector.<IPlanningAction>; // IEnumerable<IPlanningAction<State>>
			backing_plan( { OwenTemperature:REF_TEMP, BowlTemperature:REF_TEMP, Credits:10 }, {PieIsReadyForEat:true});
			plan = MakePlan();
			trace ("Plan:\n" + Planner.planToString(plan, _initialState));
		}

	}	
}
