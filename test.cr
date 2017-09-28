(def fac (fun
  (n)
  (if (= 0 n)
    1
    (* n
       (fac (- n 1))))))

(fac 5)
