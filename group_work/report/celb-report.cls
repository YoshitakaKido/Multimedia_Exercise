%
% 計算機工学実験Bレポート作成用クラスファイル
% celb-reprot.cls
\ProvidesClass{celb-report}[2017/03/08 計算機工学実験Bレポート]

%
% jsarticleの読み込み
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{jsarticle}}
\ProcessOptions\relax
\LoadClass{jsarticle}

%
% Load packages
\RequirePackage[top=10truemm,bottom=20truemm,left=15truemm,right=15truemm]{geometry}
\RequirePackage[dvipdfmx]{color}
\RequirePackage[dvipdfmx]{graphicx}
\RequirePackage{listings, jlisting}
\RequirePackage{url}

% ページスタイル
\pagestyle{plain}

% 行間の設定
\def\baselinestretch{1}

% タイトル部の設定
\def\period#1{\gdef\@period{#1}}
\def\stunum#1{\gdef\@stunum{#1}}
\def\maketitle{
\noindent \textbf{\Large 計算機工学実験B 第\@period レポート}
\begin{flushright}
提出者　\@stunum ~~~ \@author \\
提出日　\@date
\end{flushright}
}

% Sectionの設定
\def\section{\@startsection{section}{1}{-3ex}
{-3.5ex plus -1ex minus -.2ex}{1.5ex plus .2ex}{\large \bf}}
%\renewcommand{\thesection}{}

% SubSectionの設定
\def\subsection{\@startsection{subsection}{1}{\z@}
{.8ex plus .8ex minus .2ex}{.05ex plus .07ex}{\bf}}

% SubSubSectionの設定
\def\subsubsection{\@startsection{subsubsection}{1}{\z@}
{.8ex plus .8ex minus .2ex}{.05ex plus .07ex}{}}

% kadaiの設定
\def\kadai{\@startsection{paragraph}{1}{\z@}
{.8ex plus .8ex minus .2ex}{.05ex plus .07ex}{}}
\renewcommand{\theparagraph}{(\arabic{paragraph})}

% thebibliography見出しの設定（Section設定の影響を排除）
\renewcommand{\refname}{\hspace{3ex}参考文献}
