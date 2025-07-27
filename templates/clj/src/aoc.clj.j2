(ns aoc
  "Helper library functions for Advent of Code challenges."
  (:require [clojure.java.io :as io])
  (:gen-class))

(def ^:dynamic *input-dir* "resources")

(defn read-input
  "Read input file located in *input-dir* directory with the given  day number, day-num.
  The filename format: day_##.txt"
  ([day-num]
   (read-input day-num :solution))
  ([day-num type]
   (assert (and (> day-num 0)
                (<= day-num 25)))
   (let [fname (str *input-dir* "/day_"
                    (if (< day-num 10)
                      (str "0" day-num)
                      day-num)
                    (cond
                      (= type :solution) (str)
                      (= type :test) "_test"
                      :else (assert false "Only :solution & :test are accepted for type arg."))
                    ".txt")]
     (with-open [rdr (io/reader fname)]
       (->> rdr
            (line-seq)
            (doall))))))

(defn num->digits
  "Convert number n to a seq of individual digits"
  [n]
  (map #(Character/digit % 10) (str n)))

(defn digit-count
  "Return the number of digits in the number n.
  https://brilliant.org/wiki/finding-digits-of-a-number/"
  [n]
  (+ (int (Math/floor (Math/log10 n))) 1))

(defn str->numbers
  [line]
  (mapv parse-long (re-seq #"\d+" line)))

