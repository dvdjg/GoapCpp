package Goap.PrioritizedCollections
{
	import Goap.Graph.Path;

	public interface IPrioritized
	{
		function get isEmpty():Boolean;
		function get length():int;
		function push(path:Path):void;
		function pushLazy(path:Path):void;
		function pop():Path;
		function clear():void; 
	}	
}
