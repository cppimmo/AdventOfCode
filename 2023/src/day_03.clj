(ns day-03
  (:require (aoc)
            [clojure.string :as str]))

(def input (aoc/read-input 3))
(comment
  ;; The input does have duplicates
  (count (remove #(not (identity %))
                 (map (fn [elem]
                        (apply distinct? (re-seq #"\d+" elem)))
                      day-03/input))))

;;; When iterating through the input compare the currently line to the line above
;;; or below it if applicable.
;;;
;;; 467..114..
;;; ...*......
;;; ..35..633.
;;; ......#...
;;; 617*......
;;; .....+.58.
;;; ..592.....
;;; ......755.
;;; ...$.*....
;;; .664.598..

(def test-input (list "467..114.."
                      "...*......"
                      "..35..633."
                      "......#..."
                      "617*......"
                      ".....+.58."
                      "..592....."
                      "......755."
                      "...$.*...."
                      ".664.598.."))

(defn re-find-idxs
  ([re s]
   (re-find-idxs re s nil))
  ([re s parser]
   (sort #(< (first %1) (first %2))
         (loop [start-from 0
                matches ()]
           (if (>= start-from (count s))
             matches
             (let [matcher (re-matcher re (subs s start-from))
                   match (re-find matcher)]
               (if match
                 (let [digit-count (count match)
                       idx-of (+ start-from (.start matcher))]
                   (recur (+ idx-of (inc digit-count))
                          (conj matches [idx-of (if parser
                                                  (parser match)
                                                  match)])))
                 (recur (inc start-from) matches))))))))

(def part-number-idxs (fn [s] (re-find-idxs #"\d+" s parse-long)))

(defn schematic-symbol?
  "Test if character or string is classified as a symbol on the schematic.
  Type may be set to 'any or 'gear."
  [char type]                           ; 'any or 'gear
  (not= (re-find (cond
                   (= type 'any) #"[^\.0-9]"
                   (= type 'gear) #"\*"
                   :else (assert true))
                 (str char)) nil))

(defn valid-part-number?
  "Determine if a symbol is found surrounding a potential part number."
  [[idx-of num]          ; Destructure part number
   line]                 ; Line may be the above, same, or below line
  ;; Search area looks like this:
  ;; ..... (above line)
  ;; .457. (same line as potential part number)
  ;; ..... (below line)
  (some #(schematic-symbol? % 'any)
        (let [digi-cnt (aoc/digit-count num)]
          (subs line
                (if (not= idx-of 0)
                  (dec idx-of)
                  idx-of)
                (if (> (+ idx-of (inc digi-cnt)) (count line))
                  (+ idx-of digi-cnt)
                  (+ idx-of (inc digi-cnt)))))))

(defn part-numbers [input]
  (let [cnt (count input)]
    (map-indexed (fn [idx s]
                   ;;(println idx s)
                   (cond
                     (< idx 1) (distinct ; First
                                (concat (filter #(valid-part-number? % s) (part-number-idxs s))
                                        (filter #(valid-part-number? % (nth input (inc idx))) (part-number-idxs s))))
                     (= idx (dec cnt)) (distinct ; Last
                                        (concat (filter #(valid-part-number? % (nth input (dec idx))) (part-number-idxs s))
                                                (filter #(valid-part-number? % s) (part-number-idxs s))))
                     :else (distinct ; First nor last
                            (concat (filter #(valid-part-number? % (nth input (dec idx))) (part-number-idxs s))
                                    (filter #(valid-part-number? % s) (part-number-idxs s))
                                    (filter #(valid-part-number? % (nth input (inc idx))) (part-number-idxs s))))))
                 input)))

(def sum-part-numbers
  (comp
   (partial reduce (fn [acc part-nums]
                     (+ acc (reduce + 0 (map (fn [elem]
                                              (second elem)) part-nums))))
            0)
   part-numbers))

(defn solve
  []
  (sum-part-numbers input))

(defn gear-idxs
  [input]
  (map-indexed (fn [idx line]
                 (vector idx (re-find-idxs #"\*" line)))
               input))

(println "Answer 1:" (solve))

