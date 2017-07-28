
-- consider a function sum which adds together values, much like +
-- (exactly like + infact)
(def sum
  (fun (& args) -- we want to handle an arbitrary number of arguments so we use
                -- the rest symbol, "&", which gathers the overflow of arguments
                -- in one list called args, or whatever we want to call it
    (if (= args '())
      0 -- if the args list is empty we return 0
      (+ (head args)
         (sum (tail args))) -- otherwise we add together the first argument
                            -- and the sum of the remaining arguments

(sum 1 2 3) -- returns 6

-- if we have a function sub which subtracts values from the first value
-- we can bind the first value to a name and then gather the rest in a list

(def sub
  (fun (val & args)
    (if (= args '())
      val -- if we have no more subtractors we return the value
      (sub (- val (head args)) (tail args)) -- otherwise we subtract one arg
                                            -- and then repeat with our new value

(sub 5 1 1 1) -- returns 2
