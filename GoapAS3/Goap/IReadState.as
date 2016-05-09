package Goap
{
	/**
	 * A world state
	 */
	public interface IReadState extends IHashable
	{
		function getMap():Object; /// Read-only member list
		function get length():int;
		function at(key:String):*;
		function equals(other:IReadState):Boolean;
		function getCost():Number;
		function clone():IWriteState;
		function toString():String;
	}
}
