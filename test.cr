(def y 12)

(def gen_inc (fun
  '(x)
  '(fun '(y) '(+ x y))))

(def incy (gen_inc y))

(print (incy 10))

(print '(hello world))
