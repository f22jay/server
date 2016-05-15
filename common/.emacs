(add-to-list 'load-path "~/.emacs.d/")  
(add-to-list 'load-path "~/.emacs.d/plugins")  
(add-to-list 'custom-theme-load-path "~/.emacs.d/themes")
;; turn on font-lock mode
(when (fboundp 'global-font-lock-mode)
  (global-font-lock-mode t))

;; enable visual feedback on selections
                                        ;(setq transient-mark-mode t)

;; default to better frame titles
(setq frame-title-format
      (concat  "%b - emacs@" system-name))

(global-set-key (kbd "M-4") 'delete-window)
(global-set-key (kbd "M-1") 'delete-other-windows)
(global-set-key (kbd "M-2") 'split-window-vertically)
(global-set-key (kbd "M-3") 'split-window-horizontally)
(global-set-key (kbd "M-0") 'other-window)
(global-set-key (kbd "M-s s") 'shell)
(global-set-key (kbd "\C-xf") 'find-file)
(global-set-key ( kbd "C-\\") 'redo)
(global-set-key ( kbd "C-/") 'undo)
(global-set-key ( kbd "C-_") 'undo) 
(global-set-key (kbd "C-^") 'revert-buffer)
(global-set-key "\M-r" 'replace-string)
(global-set-key (kbd "M-s 3") 'comment-or-uncomment-region)  
(global-set-key (kbd "M-s 1") 'insert-baidu-comment-1)

;;========================================
;; 缓冲区
;;========================================

;;设定行距
(setq default-line-spacing 0)

;;页宽
(setq default-fill-column 90)

;;缺省模式 text-mode
;; (setq default-major-mode 'text-mode)

;;设置删除纪录
(setq kill-ring-max 200)

;;以空行结束
(setq require-final-newline t)


;;语法加亮
(global-font-lock-mode t)

;;高亮显示区域选择
(transient-mark-mode t)

                                        ;高亮显示成对括号，但不来回弹跳
(show-paren-mode t)
(setq show-paren-style 'parentheses)

;;鼠标指针规避光标
                                        ;(mouse-avoidance-mode 'animate)

;;粘贴于光标处，而不是鼠标指针处
(setq mouse-yank-at-point t)

;;========================================
;; 回显区
;;========================================

;;打开闪屏报警
;; (setq visible-bell t)
;; 关闭闪屏报警
(setq visible-bell nil)

;;使用 y or n 提问
(fset 'yes-or-no-p 'y-or-n-p)

;;锁定行高
(setq resize-mini-windows nil)

;;递归 minibuffer
(setq enable-recursive-minibuffers t)

;; 当使用 M-x COMMAND 后，过 1 秒钟显示该 COMMAND 绑定的键。
;;(setq suggest-key-bindings 1) ;;

;;========================================
;; 状态栏
;;========================================

;;显示时间
(display-time)
;;时间格式
(setq display-time-24hr-format t)
(setq display-time-day-and-date t)
(setq display-time-interval 10)

;;显示列号
(setq column-number-mode t)

;;标题栏显示 %f 缓冲区完整路径 %p 页面百分数 %l 行号
(setq frame-title-format "%f")


;;========================================
;; 编辑器设定
;;========================================

;;不生成临时文件
;;(setq-default make-backup-files nil)


;;只渲染当前屏幕语法高亮，加快显示速度
(setq font-lock-maximum-decoration t)

;;将错误信息显示在回显区
                                        ;(condition-case err
                                        ;    (progn
                                        ;    (require 'xxx) )
                                        ;  (error
                                        ;   (message "Can't load xxx-mode %s" (cdr err))))

;;使用X剪贴板
(setq x-select-enable-clipboard t)
;;;;;;;; 使用空格缩进 ;;;;;;;;
;; indent-tabs-mode  t 使用 TAB 作格式化字符  nil 使用空格作格式化字符
(setq indent-tabs-mode nil)
(setq tab-always-indent nil)
(setq tab-width 4)
(setq default-tab-width 4)
(setq-default indent-tabs-mode nil)

(provide 'redo)

(defvar redo-version "1.02"
  "Version number for the Redo package.")

(defvar last-buffer-undo-list nil
  "The head of buffer-undo-list at the last time an undo or redo was done.")
(make-variable-buffer-local 'last-buffer-undo-list)

(make-variable-buffer-local 'pending-undo-list)

;; Emacs 20 variable
(defvar undo-in-progress)

(defun redo (&optional count)
  "Redo the the most recent undo.
Prefix arg COUNT means redo the COUNT most recent undos.
If you have modified the buffer since the last redo or undo,
then you cannot redo any undos before then."
  (interactive "*p")
  (if (eq buffer-undo-list t)
      (error "No undo information in this buffer"))
  (if (eq last-buffer-undo-list nil)
      (error "No undos to redo"))
  (or (eq last-buffer-undo-list buffer-undo-list)
      ;; skip one undo boundary and all point setting commands up
      ;; until the next undo boundary and try again.
      (let ((p buffer-undo-list))
        (and (null (car-safe p)) (setq p (cdr-safe p)))
        (while (and p (integerp (car-safe p)))
          (setq p (cdr-safe p)))
        (eq last-buffer-undo-list p))
      (error "Buffer modified since last undo/redo, cannot redo"))
  (and (or (eq buffer-undo-list pending-undo-list)
           (eq (cdr buffer-undo-list) pending-undo-list))
       (error "No further undos to redo in this buffer"))
  (or (eq (selected-window) (minibuffer-window))
      (message "Redo..."))
  (let ((modified (buffer-modified-p))
        (undo-in-progress t)
        (recent-save (recent-auto-save-p))
        (old-undo-list buffer-undo-list)
        (p (cdr buffer-undo-list))
        (records-between 0))
    ;; count the number of undo records between the head of the
    ;; undo chain and the pointer to the next change.  Note that
    ;; by `record' we mean clumps of change records, not the
    ;; boundary records.  The number of records will always be a
    ;; multiple of 2, because an undo moves the pending pointer
    ;; forward one record and prepend a record to the head of the
    ;; chain.  Thus the separation always increases by two.  When
    ;; we decrease it we will decrease it by a multiple of 2
    ;; also.
    (while p
      (cond ((eq p pending-undo-list)
             (setq p nil))
            ((null (car p))
             (setq records-between (1+ records-between))
             (setq p (cdr p)))
            (t
             (setq p (cdr p)))))
    ;; we're off by one if pending pointer is nil, because there
    ;; was no boundary record in front of it to count.
    (and (null pending-undo-list)
         (setq records-between (1+ records-between)))
    ;; don't allow the user to redo more undos than exist.
    ;; only half the records between the list head and the pending
    ;; pointer are undos that are a part of this command chain.
    (setq count (min (/ records-between 2) count)
          p (primitive-undo (1+ count) buffer-undo-list))
    (if (eq p old-undo-list)
        nil ;; nothing happened
      ;; set buffer-undo-list to the new undo list.  if has been
      ;; shortened by `count' records.
      (setq buffer-undo-list p)
      ;; primitive-undo returns a list without a leading undo
      ;; boundary.  add one.
      (undo-boundary)
      ;; now move the pending pointer backward in the undo list
      ;; to reflect the redo.  sure would be nice if this list
      ;; were doubly linked, but no... so we have to run down the
      ;; list from the head and stop at the right place.
      (let ((n (- records-between count)))
        (setq p (cdr old-undo-list))
        (while (and p (> n 0))
          (if (null (car p))
              (setq n (1- n)))
          (setq p (cdr p)))
        (setq pending-undo-list p)))
    (and modified (not (buffer-modified-p))
         (delete-auto-save-file-if-necessary recent-save))
    (or (eq (selected-window) (minibuffer-window))
        (message "Redo!"))
    (setq last-buffer-undo-list buffer-undo-list)))

(defun undo (&optional arg)
  "Undo some previous changes.
Repeat this command to undo more changes.
A numeric argument serves as a repeat count."
  (interactive "*p")
  (let ((modified (buffer-modified-p))
        (recent-save (recent-auto-save-p)))
    (or (eq (selected-window) (minibuffer-window))
        (message "Undo..."))
    (or (eq last-buffer-undo-list buffer-undo-list)
        ;; skip one undo boundary and all point setting commands up
        ;; until the next undo boundary and try again.
        (let ((p buffer-undo-list))
          (and (null (car-safe p)) (setq p (cdr-safe p)))
          (while (and p (integerp (car-safe p)))
            (setq p (cdr-safe p)))
          (eq last-buffer-undo-list p))
        (progn (undo-start)
               (undo-more 1)))
    (undo-more (or arg 1))
    ;; Don't specify a position in the undo record for the undo command.
    ;; Instead, undoing this should move point to where the change is.
    ;;
    ;;;; The old code for this was mad!  It deleted all set-point
    ;;;; references to the position from the whole undo list,
    ;;;; instead of just the cells from the beginning to the next
    ;;;; undo boundary.  This does what I think the other code
    ;;;; meant to do.
    (let ((list buffer-undo-list)
    	  (prev nil))
      (while (and list (not (null (car list))))
    	(if (integerp (car list))
    	    (if prev
                (setcdr prev (cdr list))
    	      ;; impossible now, but maybe not in the future
    	      (setq buffer-undo-list (cdr list))))
    	(setq prev list
    	      list (cdr list))))
    (and modified (not (buffer-modified-p))
         (delete-auto-save-file-if-necessary recent-save)))
  (or (eq (selected-window) (minibuffer-window))
      (message "Undo!"))
  (setq last-buffer-undo-list buffer-undo-list))



;;;; Smart copy, if no region active, it simply copy the current whole line
(defadvice kill-line (before check-position activate)
  (if (member major-mode
              '(emacs-lisp-mode scheme-mode lisp-mode
                                c-mode c++-mode objc-mode js-mode
                                latex-mode plain-tex-mode))
      (if (and (eolp) (not (bolp)))
          (progn (forward-char 1)
                 (just-one-space 0)
                 (backward-char 1)))))

(defadvice kill-ring-save (before slick-copy activate compile)
  "When called interactively with no active region, copy a single line instead."
  (interactive (if mark-active (list (region-beginning) (region-end))
                 (message "Copied line")
                 (list (line-beginning-position)
                       (line-beginning-position 2)))))

(defadvice kill-region (before slick-cut activate compile)
  "When called interactively with no active region, kill a single line instead."
  (interactive
   (if mark-active (list (region-beginning) (region-end))
     (list (line-beginning-position)
           (line-beginning-position 2)))))

;; Copy line from point to the end, exclude the line break
(defun qiang-copy-line (arg)
  "Copy lines (as many as prefix argument) in the kill ring"
  (interactive "p")
  (kill-ring-save (point)
                  (line-end-position))
  ;; (line-beginning-position (+ 1 arg)))
  (message "%d line%s copied" arg (if (= 1 arg) "" "s")))

(global-set-key (kbd "M-k") 'qiang-copy-line)


;;solarized
(add-to-list 'custom-theme-load-path "~/.emacs.d/themes/emacs-solarized/") 

;; material
(add-to-list 'custom-theme-load-path "~/.emacs.d/themes/emacs-material-theme")

;; (load-theme 'solarized  t)          
(load-theme 'molokai t)                
;; (load-theme 'material t)

;; (require 'package)
;; (add-to-list 'package-archives
;;              '("melpa" . "http://stable.melpa.org/packages/") t)
;; (package-initialize)

(require 'package)
(setq package-archives
      '(("gnu" . "http://elpa.gnu.org/packages/")
                                        ("melpa" . "http://melpa.org/packages")))
                                        ;("marmalade" . "http://marmalade-repo.org/packages/")))
(package-initialize)

;;swbuf
(require 'swbuff)
(global-set-key (kbd "M--") 'swbuff-switch-to-previous-buffer)
(global-set-key (kbd "M-=") 'swbuff-switch-to-next-buffer)
(setq swbuff-exclude-buffer-regexps 
      '("^ " "\\*.*\\*"))

(setq swbuff-status-window-layout 'scroll)
(setq swbuff-clear-delay 1)
(setq swbuff-separator "|")
(setq swbuff-window-min-text-height 1)

(require 'yasnippet)
(yas-global-mode 1)

;;autopairs
(require 'autopair)
(autopair-global-mode) ;; enable autopair in all buffers

;; 删除尾部空格
(global-set-key (kbd "C-<") 'delete-trailing-whitespace)

;;高亮当前编辑的行
(require 'hl-line)

;;set C indent-style
;; (add-hook 'c-mode-hook 'linux-c-mode)
;; (add-hook 'c++-mode-hook 'linux-cpp-mode)

;; (defun linux-c-mode ()
;;   (define-key c-mode-map [return] 'newline-and-indent)
;;   (interactive)
;;   (c-set-style "K&R")
;;   (c-toggle-hungry-state)
;;   (setq tab-width 4)
;;   (setq indent-tabs-mode nil)
;;   (setq c-basic-offset 4)
;;   (imenu-add-menubar-index)
;;   (which-function-mode 1))
;; ;; (c-toggle-auto-state)  ;;关闭;自动换行

;; (defun linux-cpp-mode()
;;   (define-key c++-mode-map [return] 'newline-and-indent)
;;   (interactive)
;;   (c-set-style "stroustrup")
;;   (c-toggle-hungry-state)
;;   (setq tab-width 4)
;;   (setq indent-tabs-mode nil)
;;   (setq c-basic-offset 4)
;;   (imenu-add-menubar-index)
;;   (which-function-mode 1))
;; (c-toggle-auto-state)

;; <F11> -> start grep
(define-key global-map [(f11)] 'rgrep)

;; <F6> -> start find
(define-key global-map [(f6)] 'find-dired)


;; set the default test coding system
(setq default-buffer-file-coding-system 'utf-8)

;; <F9> -> Save all file
;;(define-key global-map [(f9)] 'save-some-buffers)
;; <F9> -> linum-mode
(define-key global-map [(f9)] 'linum-mode)

;; insert-current-time
(defun insert-current-date ()
  "Insert the current date"
  (interactive "*")
  (insert (format-time-string "%Y/%m/%d %H:%M:%S" (current-time))))
;; (insert (format-time-string "%Y/%m/%d" (current-time))))
(global-set-key "\C-xt" 'insert-current-date)


;; evim for get features like vim
                                        ;(add-to-list 'load-path "~/.emacs.d/evil-evil/")
                                        ;(require 'evil)
                                        ;(evil-mode 0)

(set-face-attribute  'mode-line
                     nil 
                     ;; :foreground "green"
                     ;; :background "brown"
                     :foreground "red"
                     :background "white"
                     :box '(:line-width 1 :style released-button))
(set-face-attribute  'mode-line-inactive
                     nil
                     :foreground "blue"
                     :background "white" 
                     :box '(:line-width 1 :style released-button))

(defun insert-baidu-comment-1()  
  (interactive)
  (insert "// Copyright 2016  Inc. All Rights Reserved.
// Author : jay (f22jay@gmail.com)
// 
// Breif :
"))
(global-set-key (kbd "M-s c 1") 'insert-baidu-comment-1)


(defun insert-baidu-comment-2()  
  (interactive)
  (insert (message "/** **************************************************************************
*
*         Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
* 
x* ****************************************************************************
*/

/**
* @File %s
* @Author moon <f22jay@gmail.com>
* @Date %s
* @Version 1.0
* @Brief : 
**/
" (current-buffer) (format-time-string "%Y/%m/%d %H:%M:%S" (current-time)))))
(global-set-key (kbd "M-s c 2") 'insert-baidu-comment-2)


;;格式化整个文件函数
(defun indent-whole ()
  (interactive)
  (indent-region (point-min) (point-max))
  (message "format successfully"))
;;绑定到F7键
(global-set-key [f7] 'indent-whole)

(defun qiang-comment-dwim-line (&optional arg)
  "Replacement for the comment-dwim command. If no region is selected and current line is not blank and we are not at the end of the line, then comment current line. Replaces default behaviour of comment-dwim, when it inserts comment at the end of the line."
  (interactive "*P")
  (comment-normalize-vars)
  (if (and (not (region-active-p)) (not (looking-at "[ \t]*$")))
      (comment-or-uncomment-region (line-beginning-position) (line-end-position))
    (comment-dwim arg)))
(global-set-key "\M-;" 'qiang-comment-dwim-line) 

;; 自动为 C/C++ 的头文件添加 #define 保护。
(define-auto-insert
  '("\\.\\([Hh]\\|hh\\|hxx\\|hpp\\)\\'" . "C / C++ header")
  '((upcase (concat "_"
                    (replace-regexp-in-string
                     "[^a-zA-Z0-9]" "_"
                     (format "%s_%d_" (file-name-nondirectory buffer-file-name) (random)))))
    "#ifndef " str \n
    "#define " str "\n\n"
    _ "\n\n#endif"))

;; the following elisp code is use to
;; construct the compile command automatically.
;; so you don't need to write or choose the compile
;; command in the minibuffer.
;;
;; the rule is as following:
;; first, find GNUmakefile" "makefile" "Makefile"
;; if found, compile command is "make -C ."
;; else try to find one of make file in ".."
;; if found, compile command is "make -C .."
;; continue until reach the root directory "/"
;; if the make file still isn't found, try the
;; trivial compile command , that is,
;; if in the c++ mode:
;;         g++ -o your_source your_source.cc
;; if in the c mode
;;         gcc -o your_source your_source.c
;; if all is failed, do nothing.
;;
;; if you like this command, you can put the following
;; line in your .emacs
;;
;; (global-set-key (kbd "<f7>") 'smart-compile)
;;

(defun smart-compile-is-root-dir(try-dir)
  (or
   ;; windows root dir for a driver or Unix root
   (string-match "\\`\\([a-zA-Z]:\\)?/$" try-dir)
   ;; tramp root-dir
   (and (featurep 'tramp)
        (string-match (concat tramp-file-name-regexp ".*:/$") try-dir))))
(defun smart-compile-throw-final-path(try-dir)
  (cond
   ;; tramp root-dir
   ((and (featurep 'tramp)
         (string-match tramp-file-name-regexp try-dir))
    (with-parsed-tramp-file-name try-dir foo
                                 foo-localname))
   (t try-dir)))

(defun smart-compile-find-make-dir( try-dir)
  "return a directory contain makefile. try-dir is absolute path."
  (if (smart-compile-is-root-dir try-dir)
      nil ;; return nil if failed to find such directory.
    (let ((candidate-make-file-name `("GNUmakefile" "makefile" "Makefile")))
      (or (catch 'break
            (mapc (lambda (f)
                    (if (file-readable-p (concat (file-name-as-directory try-dir) f))
                        (throw 'break (smart-compile-throw-final-path try-dir))))
                  candidate-make-file-name)
            nil)
          (smart-compile-find-make-dir
           (expand-file-name (concat (file-name-as-directory try-dir) "..")))))))


(defun wcy-tramp-compile (arg-cmd)
  "reimplement the remote compile."
  (interactive "scompile:")
  (with-parsed-tramp-file-name default-directory foo
                               (let* ((key (format "/plink:%s@%s:" foo-user foo-host))
                                      (passwd (password-read "PASS:" key))
                                      (cmd (format "plink %s -l %s -pw %s \"(cd %s ; %s)\""
                                                   foo-host foo-user
                                                   passwd
                                                   (file-name-directory foo-localname)
                                                   arg-cmd)))
                                 (password-cache-add key passwd)
                                 (save-some-buffers nil nil)
                                 (compile-internal cmd "No more errors")
                                 ;; Set comint-file-name-prefix in the compilation buffer so
                                 ;; compilation-parse-errors will find referenced files by ange-ftp.
                                 (with-current-buffer compilation-last-buffer
                                   (set (make-local-variable 'comint-file-name-prefix)
                                        (format "/plink:%s@%s:" foo-user foo-host))))))
(defun smart-compile-test-tramp-compile()
  (or (and (featurep 'tramp)
           (string-match tramp-file-name-regexp (buffer-file-name))
           (progn
             (if (not (featurep 'tramp-util)) (require 'tramp-util))
             'wcy-tramp-compile))
      'compile))
(defun smart-compile-get-local-file-name(file-name)
  (if (and
       (featurep 'tramp)
       (string-match tramp-file-name-regexp file-name))
      (with-parsed-tramp-file-name file-name foo
                                   foo-localname)
    file-name))
(defun smart-compile ()
  (interactive)
  (let* ((compile-func (smart-compile-test-tramp-compile))
         (dir (smart-compile-find-make-dir (expand-file-name "."))))
    (funcall compile-func
             (if dir
                 (concat "make -C " dir (if (eq compile-func 'tramp-compile) "&" ""))
               (concat
                (cond
                 ((eq major-mode 'c++-mode) "g++ -g -o ")
                 ((eq major-mode 'c-mode) "gcc -g -o "))
                (smart-compile-get-local-file-name (file-name-sans-extension (buffer-file-name)))
                " "
                (smart-compile-get-local-file-name (buffer-file-name)))))))"))"

(global-set-key (kbd "<f8>") 'smart-compile)

;; C-x C-j open the directory of current buffer
(global-set-key (kbd "C-x C-j")
                (lambda ()
                  (interactive)
                  (if (buffer-file-name)
                      (dired default-directory))))

(setq wcy-shell-mode-directory-changed t)

(defun wcy-shell-mode-auto-rename-buffer-output-filter (text)
  (if (and (eq major-mode 'shell-mode)
           wcy-shell-mode-directory-changed)
      (progn
        (let ((bn  (concat "sh:" default-directory)))
          (if (not (string= (buffer-name) bn))
              (rename-buffer bn t)))
        (setq wcy-shell-mode-directory-changed nil))))


(defun wcy-shell-mode-auto-rename-buffer-input-filter (text)
  (if (eq major-mode 'shell-mode)
      (if ( string-match "^[ \t]*cd *" text)
          (setq wcy-shell-mode-directory-changed t))))
(add-hook 'comint-output-filter-functions 'wcy-shell-mode-auto-rename-buffer-output-filter)
(add-hook 'comint-input-filter-functions 'wcy-shell-mode-auto-rename-buffer-input-filter )
;;exit shell and exit buffer
(add-hook 'shell-mode-hook 'wcy-shell-mode-hook-func)
(defun wcy-shell-mode-hook-func  ()
  (set-process-sentinel (get-buffer-process (current-buffer))
                        #'wcy-shell-mode-kill-buffer-on-exit)
  )
(defun wcy-shell-mode-kill-buffer-on-exit (process state)
  (message "%s" state)
  (if (or
       (string-match "exited abnormally with code.*" state)
       (string-match "finished" state))
      (kill-buffer (current-buffer))))

                                        ;ido                    


;; (prefer-coding-system 'gbk) ; 此时buffer新建和读取都默认是gbk,也可以M-x prefer-coding-system 只执行一次
;; (setq default-buffer-file-coding-system 'gbk)



;; clang
                                        ; auto complete;


;; (column-number-mode t)                  
;; (setq column-number-mode t)
(global-linum-mode 1)

;;flyspell
;; (dolist (hook '(text-mode-hook))
;;   (add-hook hook (lambda () (flyspell-mode 1))))
;; (dolist (hook '(change-log-mode-hook log-edit-mode-hook))
;;   (add-hook hook (lambda () (flyspell-mode -1))))

;;elpy
(elpy-enable)

(auto-complete-mode t)
(require 'ido)
(ido-mode t)

                                        ;smex
(require 'smex) ; Not needed ;; if you use package.el
(smex-initialize) ; 
(global-set-key (kbd "M-x") 'smex)
(global-set-key (kbd "M-X") 'smex-major-mode-commands)
;; This is your old M-x.
(global-set-key (kbd "C-c C-c M-x") 'execute-extended-command)

;;auto-complete must here, othere need M-x auto-complete-mode can complete
;; (require 'auto-complete-config)
;; (ac-config-default)
;; (require 'ac-clang)

;; (when (ac-clang-initialize)
;;   (add-hook 'c-mode-common-hook '(lambda ()
;;                                    (setq ac-clang-cflags CFLAGS)
;;                                    (ac-clang-activate-after-modify))))
;; (ac-clang-initialize)

;; ;; (ac-clang-activate-after-modify)
;; (ac-clang-activate)
;; ;; ac-source-gtags
;; ;; (my-ac-config)

(add-to-list 'load-path "~/.emacs.d/plugins/helm")  

(require 'helm)
(require 'helm-config)

;; The default "C-x c" is quite close to "C-x C-c", which quits Emacs.
;; Changed to "C-c h". Note: We must set "C-c h" globally, because we
;; cannot change `helm-command-prefix-key' once `helm-config' is loaded.
(global-set-key (kbd "C-c h") 'helm-command-prefix)
(global-unset-key (kbd "C-x c"))

(define-key helm-map (kbd "<tab>") 'helm-execute-persistent-action) ; rebind tab to run persistent action
(define-key helm-map (kbd "C-i") 'helm-execute-persistent-action) ; make TAB works in terminal
(define-key helm-map (kbd "C-z")  'helm-select-action) ; list actions using C-z

(when (executable-find "curl")
  (setq helm-google-suggest-use-curl-p t))

(setq helm-split-window-in-side-p           t ; open helm buffer inside current window, not occupy whole other window
      helm-move-to-line-cycle-in-source     t ; move to end or beginning of source when reaching top or bottom of source.
      helm-ff-search-library-in-sexp        t ; search for library in `require' and `declare-function' sexp.
      helm-scroll-amount                    8 ; scroll 8 lines other window using M-<next>/M-<prior>
      helm-ff-file-name-history-use-recentf t)

(helm-mode 1)
(global-set-key (kbd "M-x") 'helm-M-x)

(global-set-key (kbd "C-x C-f") 'helm-find-files)
(global-set-key (kbd "C-x b") 'helm-mini)
(global-set-key (kbd "C-x C-b") 'helm-mini)
(global-set-key (kbd "C-c h o") 'helm-occur)
(global-set-key (kbd "C-c h f") 'helm-find)

;;mohu pipei
(helm-autoresize-mode t)
(setq helm-M-x-fuzzy-match t)
(setq helm-buffers-fuzzy-matching t
      helm-recentf-fuzzy-match    t)
;;go definition后， 返回上次的tag, 
(global-set-key (kbd "M-p") 'pop-tag-mark)

(global-set-key (kbd"C-c g") 'gdb) ; 编译
(global-set-key (kbd"C-c c") 'compile) ; 编译

;; (global-set-key (kbd "C-q") 'undo)
;; (global-set-key (kbd "M-a") 'backward-char) ; Meta+a
;; ;; 
(define-auto-insert 'sh-mode '(nil "#!/usr/bin/env bash\n\n"))
(add-hook 'find-file-hooks 'auto-insert)

;; company;; -mode                         
;; (add-to-list 'load-path "~/.emacs.d/elpa/company")
;; (autoload 'company-mode "company" nil t)
;; ;; 代码补全
;; (add-hook 'c-mode-hook 'company-mode)
;; (add-hook 'c++-mode-hook 'company-mode)
;; (setq company-clang-arguments '(
;; "/usr/include/"
;; "/usr/local/include"
;; "/home/moon/src/common/include"
;; ))

;; (add-to-list 'load-path "~/.emacs.d/auto-complete-1.3.1")  
(require 'auto-complete)  
;; (add-to-list 'ac-dictionary-directories "~/.emacs.d/auto-complete-1.3.1/ac-dict")
 (add-to-list 'ac-dictionary-directories "~/.emacs.d/elpa/auto-complete-20160329/dict")

(require 'auto-complete-config)
(auto-complete-mode t)
(ac-config-default)
(require 'google-c-style)
(add-hook 'c-mode-hook 'google-set-c-style)
(add-hook 'c++-mode-hook 'google-set-c-style)
(add-hook 'c-mode-common-hook 'google-make-newline-indent)
(add-hook 'c++-mode-common-hook 'google-make-newline-indent)
(add-hook 'c++-mode-hook '(lambda () (print "hello c++")))

(require 'flymake-shell)
(add-hook 'sh-set-shell-hook 'flymake-shell-load)
(require 'flymake-python-pyflakes)
(add-hook 'python-mode-hook 'flymake-python-pyflakes-load)

(require 'helm-gtags)
(setq helm-gtags-ignore-case t
    helm-gtags-auto-update t
    helm-gtags-use-input-at-cursor t
    helm-gtags-pulse-at-cursor t
    helm-gtags-prefix-key "\C-cg"
    helm-gtags-suggested-key-mapping t)
(defun set-helm-gtags-keybindings ()
  (define-key helm-gtags-mode-map (kbd "C-c g a") 'helm-gtags-tags-in-this-function)
  (define-key helm-gtags-mode-map (kbd "C-c g s") 'helm-gtags-select)
  (define-key helm-gtags-mode-map (kbd "C-c g g") 'helm-gtags-create-tags)
  (define-key helm-gtags-mode-map (kbd "M-."	 ) 'helm-gtags-dwim)
  (define-key helm-gtags-mode-map (kbd "M-,"	 ) 'helm-gtags-pop-stack)
  (define-key helm-gtags-mode-map (kbd "C-c g p") 'helm-gtags-previous-history)
  (define-key helm-gtags-mode-map (kbd "C-c g n") 'helm-gtags-next-history))
(add-hook 'helm-gtags-mode-hook 'set-helm-gtags-keybindings)
(add-hook 'c++-mode-hook 'helm-gtags-mode)
(add-hook 'c-mode-hook 'helm-gtags-mode)

