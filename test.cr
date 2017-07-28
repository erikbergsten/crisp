(module test)

(def succ (fun (x) (+ x 1)))
(def pred (fun (x) (- x 1)))

(def length
  (fun (list)
    (if (= list '())
      0
      (+ 1 (length (tail list))))))

(def conc
  (fun (l1 l2)
    (if (= l1 '())
      l2
      (conj (head l1) (conc (tail l1) l2)))))

(def map
  (fun (fn list)
    (if (= list '())
      '()
      (conj (fn (head list)) (map fn (tail list))))))

(def list1 '(1 2 3))
(def list2 '(4 5))

(print list1)
(print (conc list1 list2))
(print (map succ list1))
