(def inc_gen
  (fun '(x)
    '(fun '(y) '(+ x y))))

(def inc5 (inc_gen 5))

(print (inc5 1))
