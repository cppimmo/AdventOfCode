(ns day-05
  (:require aoc
            [clojure.string :as str]))

(def input (aoc/read-input 5))
(def test-input (aoc/read-input 5 :test))

(defn seeds?
  [line]
  (not= (str/index-of line "seeds:") nil))

(defn fetch-seeds
  [line]
  (aoc/str->numbers (subs line (inc (str/index-of line ":")))))

;;; Source category | destination category
;;; Destination range start | source range start | range length
(def test-map [[50 98 2]
               [52 50 48]])
(def test-seeds [79 14 55 13])

(defn mapper
  "Return closure that can map a seed through the given category."
  [[dest-start src-start len]]
  (fn [n]         ; Source number
    (if (and (>= n src-start)
             (< n (+ src-start len)))
      (+ dest-start (- n src-start))
      n))) ; Simply return the number if it is not in the source range

(defn split-categories
  [input]
  (let [empty-str? #(empty? (str/trim %))]
    (->> (rest input)
         (partition-by empty-str?)
         (remove #(empty-str? (first %)))
         (mapv #(remove (fn [s]
                          (not= (str/index-of s ":") nil))
                        %))
         (mapv (fn [lines]
                 (mapv #(aoc/str->numbers %) lines))))))

(comment
  (let [seeds (fetch-seeds (first input))]
    (apply min
           (mapv (fn [seed]
                   (reduce (fn [acc f] (f acc))
                           seed
                           (mapv mapper test-map)))
                 test-seeds))))

(defn solve
  [input]
  (let [seeds (fetch-seeds (first input))
        categories (apply concat (split-categories input))
        mappers (mapv mapper categories)]
    (->> (mapv (fn [seed]
                 (reduce (fn [acc f]
                           (println acc)
                           (f acc))
                         seed
                         mappers))
               seeds)
         (sort)
         (take-while <)
         (first))))

