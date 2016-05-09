package Goap.Planning
{
	import Goap.IReadState;
	import Goap.Planning.Meter.IPlanningStateMeter;

	public interface IPlanner
	{
		function makePlan(initialState:IReadState, 
						  planningStateMeter:IPlanningStateMeter, 
						  actionsArray:Vector.<IPlanningAction> = null, 
						  states:Vector.<IReadState> = null):Vector.<IPlanningAction>;
	}	
}
