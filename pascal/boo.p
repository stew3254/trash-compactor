(* Valid program *)
program main( input, output );
  var a,b: integer;
  var c: array[1 .. 13] of integer;
  function foo(a: real): real;
  begin
    foo := a
  end;
  procedure boo(a: integer; b: real; c: integer);
    var d: real;
  begin
    d := 1.23
  end;
begin
end.
