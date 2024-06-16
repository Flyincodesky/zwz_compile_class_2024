label main
begin
var a
var b
var c
a = 1
var t0
t0 = (a < 1000)
ifz t0 goto L2
label L1
var t1
t1 = a + 1
a = t1
var t2
t2 = a * 2
b = t2
c = 5
t0 = (a < 1000)
if t0 goto L1
label L2
var t3
actual c
actual b
t3 = call add
b = t3
actual b
call PRINTN
end
label add
begin
formal x
formal y
var t4
t4 = x + y
return t4
end
