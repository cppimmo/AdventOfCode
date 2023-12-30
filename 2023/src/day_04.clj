(ns day-04
  (:require aoc
            [clojure.string :as str]
            [clojure.set :as set]))

(def input (aoc/read-input 4))
(def test-input (aoc/read-input 4 :test))

(defn parse-card
  [line]
  (let [nums (mapv (fn [elem]
                     (mapv parse-long (remove empty? (str/split elem #"\s+"))))
                   (str/split (subs line (inc (str/index-of line ":"))) #"\|"))]
    {:winning (first nums) :scratched (second nums)}))

(defn winning-count
  [{:keys [winning scratched]}]
  (count (set/intersection (set winning)
                           (set scratched))))

(defn winning-counts
  [input]
  (mapv #(winning-count (parse-card %)) input))

(defn calculate-points
  [card]  
  (let [cnt (winning-count card)]
    (if (zero? cnt)
      0
      (bit-shift-left 1 (dec cnt)))))

(defn solve
  [input]
  (reduce +
          (mapv #(calculate-points %)
                (mapv parse-card input))))

(defn solve-2
  [input]
  (comment (map-indexed
            (fn [idx win-cnt]
              (range (inc idx) (+ 2 idx win-cnt)))
            (mapv #(->> % parse-card winning-count) input))))

(println "Answer 1:" (solve input))

