-- consider a function which generates incrementing functions, called
-- incrementor_generator
(def incrementor_generator
  (fun (inc)
    (fun (x) (+ x inc)))) -- the internal function stores a reference to
                          -- the env in which it is created, the env in
                          -- which inc is bound

-- we then use this generator to create a function which increments
-- numbers by 5
(def inc5 (incrementor_generator 5))

-- when we now evaluate inc5, the inc value will be bound to 5 since the scope
-- in which inc5 was created will be stored within the function we generated
(inc5 10) -- returns 15

-- if we evaluated inc5 with a reference to the environment in which it is
-- called instead of the environment within witch it was created inc would not
-- be bound and we would get a null exception.
