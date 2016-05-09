package Goap
{
	/**
	 * A world state
	 */
	public interface IWriteState extends IReadState, IDisposable
	{
		function clearState():void;
		
		function set map(m:Object):void;
		function get map():Object; /// Read-only member list
		
		function setMap(m:Object):void;
		function put(key:String, value:*):IWriteState;
		function add(key:String, value:*):IWriteState;
		function mul(key:String, value:*):IWriteState;
		function and(key:String, value:Boolean):IWriteState;
		function or(key:String, value:Boolean):IWriteState;
		function remove(key:String):IWriteState;
		
		function get cost():Number;
		function set cost(value:Number):void;
		function setCost(value:Number):IWriteState;
		function addCost(value:Number):IWriteState
		function mulCost(value:Number):IWriteState;
	}	
}
